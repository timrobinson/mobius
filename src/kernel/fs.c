/* $Id: fs.c,v 1.11 2002/01/15 00:12:58 pavlovskii Exp $ */
#include <kernel/driver.h>
#include <kernel/fs.h>
#include <kernel/io.h>
#include <kernel/init.h>

#define DEBUG
#include <kernel/debug.h>

#include <os/rtl.h>
#include <os/defs.h>
#include <os/syscall.h>

#include <errno.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>

device_t *root;

typedef struct vfs_mount_t vfs_mount_t;
struct vfs_mount_t
{
	vfs_mount_t *prev, *next;
	wchar_t *name;
	device_t *fsd;
};

typedef struct vfs_dir_t vfs_dir_t;
struct vfs_dir_t
{
	device_t dev;
	vfs_mount_t *vfs_mount_first, *vfs_mount_last;
};

static bool VfsRequest(device_t *dev, request_t *req)
{
	vfs_dir_t *dir = (vfs_dir_t*) dev;
	request_fs_t *req_fs = (request_fs_t*) req;
	const wchar_t *ch;
	size_t len;
	vfs_mount_t *mount;

	switch (req->code)
	{
	/* Assumes params_fs_t.fs_create <=> params_fs_t.fs_open */
	case FS_CREATE:
	case FS_OPEN:
		if (req_fs->params.fs_open.name[0] == '/')
			req_fs->params.fs_open.name++;

		ch = wcschr(req_fs->params.fs_open.name, '/');
		if (ch == NULL)
			len = wcslen(req_fs->params.fs_open.name);
		else
			len = ch - req_fs->params.fs_open.name;

		/*TRACE3("Path: %s Name: %s Len: %d\n", 
			req_fs->params.fs_open.name, ch, len);*/

		FOREACH (mount, dir->vfs_mount)
			if (_wcsnicmp(mount->name, req_fs->params.fs_open.name, len) == 0)
			{
				/*TRACE1("=> %p\n", mount->fsd);*/
				req_fs->params.fs_open.name += len;
				return mount->fsd->vtbl->request(mount->fsd, req);
			}
		
		wprintf(L"%s: not found in root\n", req_fs->params.fs_open.name);
		req->result = ENOTFOUND;
		return false;
		
	case FS_MOUNT:
		if (req_fs->params.fs_mount.name[0] == '/')
			req_fs->params.fs_mount.name++;

		ch = wcschr(req_fs->params.fs_mount.name, '/');
		if (ch == NULL)
		{
			len = wcslen(req_fs->params.fs_mount.name);
			ch = req_fs->params.fs_mount.name + len;
		}
		else
			len = ch - req_fs->params.fs_mount.name;
		
		if (*ch == '\0')
		{
			/* Mounting on this directory */
			/*TRACE3("VfsRequest(FS_MOUNT): mounting %p as %*s\n",
				req_fs->params.fs_mount.fsd, 
				len,
				req_fs->params.fs_mount.name);*/

			mount = malloc(sizeof(vfs_mount_t));
			mount->name = malloc(sizeof(wchar_t) * (len + 1));
			mount->name[len] = '\0';
			memcpy(mount->name, req_fs->params.fs_mount.name, 
				sizeof(wchar_t) * len);
			mount->fsd = req_fs->params.fs_mount.fsd;
			LIST_ADD(dir->vfs_mount, mount);

			return true;
		}
		else
		{
			/* Mounting on a subdirectory */
			/*TRACE1("VfsRequest(FS_MOUNT): mounting on subdirectory %s\n", ch);*/

			FOREACH (mount, dir->vfs_mount)
				if (_wcsnicmp(mount->name, req_fs->params.fs_open.name, len) == 0)
				{
					/*TRACE1("=> %p\n", mount->fsd);*/
					req_fs->params.fs_mount.name += len;
					return mount->fsd->vtbl->request(mount->fsd, req);
				}

			req->result = ENOTFOUND;
			return false;
		}
	}

	req->result = ENOTIMPL;
	return false;
}

static void VfsFinishIo(device_t *dev, request_t *req)
{
	fileop_t *op;
	request_fs_t *req_fs;

	if (req->code == FS_READ)
		wprintf(L"VfsFinishIo: req = %p op = %p code = %x: ", 
			req, req->param, req->code);

	assert(req->code == FS_READ || req->code == FS_WRITE);
	op = req->param;
	op->result = req->result;
	req_fs = (request_fs_t*) req;
	if (req->code == FS_READ || req->code == FS_WRITE)
		op->bytes = req_fs->params.buffered.length;

	if (req->code == FS_READ)
		wprintf(L"finished io: event = %u bytes = %u result = %u\n",
			op->event, op->bytes, op->result);

	EvtSignal(NULL, op->event);
	/*free(req);*/
}

static bool FsCheckAccess(file_t *file, uint32_t mask)
{
	return (file->flags & mask) == mask;
}

/*!
 *	\brief	Creates a file
 *
 *	\param	path	Full path specification for the file
 *	\param	flags	Bitmask of access flags for the file
 *	\sa	\p FILE_READ, \p FILE_WRITE
 *	\return	Handle to the new file
 */
handle_t FsCreate(const wchar_t *path, uint32_t flags)
{
	request_fs_t req;
	wchar_t fullname[256];

	if (!FsFullPath(path, fullname))
		return NULL;

	req.header.code = FS_CREATE;
	req.params.fs_create.name = fullname;
	req.params.fs_create.name_size = (wcslen(fullname) * sizeof(wchar_t)) + 1;
	req.params.fs_create.file = NULL;
	req.params.fs_create.flags = flags;
	if (IoRequestSync(root, (request_t*) &req))
		return req.params.fs_create.file;
	else
	{
		errno = req.header.result;
		return NULL;
	}
}

/*!
 *	\brief	Opens a file
 *
 *	\param	path	Full path specification for the file
 *	\param	flags	Bitmask of access flags for the file
 *	\sa	\p FILE_READ, \p FILE_WRITE
 *	\return	Handle to the file
 */
handle_t FsOpen(const wchar_t *path, uint32_t flags)
{
	request_fs_t req;
	wchar_t fullname[256];

	if (!FsFullPath(path, fullname))
		return NULL;

	req.header.code = FS_OPEN;
	req.params.fs_open.name = fullname;
	req.params.fs_open.name_size = (wcslen(fullname) * sizeof(wchar_t)) + 1;
	req.params.fs_open.file = NULL;
	req.params.fs_open.flags = flags;
	if (IoRequestSync(root, (request_t*) &req))
		return req.params.fs_open.file;
	else
	{
		errno = req.header.result;
		return NULL;
	}
}

/*!
 *	\brief	Closes a file
 *
 *	\param	file	Handle of the file to close
 *	\return	\p true if the handle was valid
 */
bool FsClose(handle_t file)
{
	request_fs_t req;
	file_t *fd;
	device_t *fsd;

	fd = HndLock(NULL, file, 'file');
	if (fd == NULL)
		return false;

	fsd = fd->fsd;
	HndUnlock(NULL, file, 'file');

	req.header.code = FS_CLOSE;
	req.params.fs_close.file = file;
	if (IoRequestSync(fsd, (request_t*) &req))
		return true;
	else
	{
		errno = req.header.result;
		return false;
	}
}

/*!
 *	\brief	Reads from a file synchronously
 *
 *	This function will block until the read completes.
 *	\p errno will be updated with an error code if the read fails.
 *
 *	The file must have \p FILE_READ access.
 *
 *	\param	file	Handle of the file to read from
 *	\param	buf	Buffer to read into
 *	\param	bytes	Number of bytes to read
 *	\return	Number of bytes read
 */
size_t FsReadSync(handle_t file, void *buf, size_t bytes)
{
	request_fs_t req;
	file_t *fd;
	device_t *fsd;

	fd = HndLock(NULL, file, 'file');
	if (fd == NULL)
		return 0;

	if (!FsCheckAccess(fd, FILE_READ))
	{
		HndUnlock(NULL, file, 'file');
		return 0;
	}

	fsd = fd->fsd;
	HndUnlock(NULL, file, 'file');

	/*wprintf(L"FsRead: file = %lx buf = %p bytes = %lu\n",
		file, buf, bytes);*/
	req.header.code = FS_READ;
	req.params.fs_read.length = bytes;
	req.params.fs_read.buffer = buf;
	req.params.fs_read.file = file;
	if (IoRequestSync(fsd, (request_t*) &req))
		return req.params.fs_read.length;
	else
	{
		errno = req.header.result;
		return 0;
	}
}

/*!
 *	\brief	Writes to a file synchronously
 *
 *	This function will block until the write completes.
 *	\p errno will be updated with an error code if the write fails.
 *
 *	The file must have \p FILE_WRITE access.
 *
 *	\param	file	Handle of the file to write to
 *	\param	buf	Buffer to write out of
 *	\param	bytes	Number of bytes to write
 *	\return	Number of bytes written
 */
size_t FsWriteSync(handle_t file, const void *buf, size_t bytes)
{
	request_fs_t req;
	file_t *fd;
	device_t *fsd;

	fd = HndLock(NULL, file, 'file');
	if (fd == NULL)
		return 0;

	if (!FsCheckAccess(fd, FILE_WRITE))
	{
		HndUnlock(NULL, file, 'file');
		return 0;
	}

	fsd = fd->fsd;
	HndUnlock(NULL, file, 'file');

	req.header.code = FS_WRITE;
	req.params.fs_write.length = bytes;
	req.params.fs_write.buffer = buf;
	req.params.fs_write.file = file;
	if (IoRequestSync(fsd, (request_t*) &req))
		return req.params.fs_write.length;
	else
	{
		errno = req.header.result;
		return 0;
	}
}

static bool FsReadWrite(handle_t file, void *buf, size_t bytes, fileop_t *op,
						bool isRead)
{
	request_fs_t *req;
	file_t *fd;
	device_t *fsd;

	fd = HndLock(NULL, file, 'file');
	if (fd == NULL)
	{
		op->result = EHANDLE;
		return false;
	}

	if (!FsCheckAccess(fd, isRead ? FILE_READ : FILE_WRITE))
	{
		HndUnlock(NULL, file, 'file');
		op->result = EACCESS;
		op->bytes = 0;
		return false;
	}

	fsd = fd->fsd;
	HndUnlock(NULL, file, 'file');

	req = malloc(sizeof(request_fs_t));
	if (req == NULL)
	{
		op->result = ENOMEM;
		return false;
	}

	if (isRead)
		TRACE3("FsReadWrite: %s req = %p op = %p: ",
			isRead ? L"read" : L"write", req, op);

	req->header.code = isRead ? FS_READ : FS_WRITE;
	req->header.param = op;
	req->params.buffered.buffer = buf;
	req->params.buffered.length = bytes;
	req->params.buffered.file = file;
	
	if (!IoRequest(root, fsd, &req->header))
	{
		if (isRead)
			TRACE1("failed (%d)\n", req->header.result);
		op->result = req->header.result;
		op->bytes = req->params.buffered.length;
		free(req);
		EvtSignal(NULL, op->event);
		return false;
	}
	else
	{
		if (isRead)
			TRACE1("succeeded (%d)\n", req->header.result);
		op->result = req->header.result;
		return true;
	}
}

/*!
 *	\brief	Reads from a file asynchronously
 *
 *	The file must have \p FILE_READ access.
 *
 *	\param	file	Handle of the file to read from
 *	\param	buf	Buffer to read into
 *	\param	bytes	Number of bytes to read
 *	\param	op	\p fileop_t structure that receives the results of the read
 *	\return	\p true if the read could be started
 */
bool FsRead(handle_t file, void *buf, size_t bytes, struct fileop_t *op)
{
	return FsReadWrite(file, buf, bytes, op, true);
}

/*!
 *	\brief	Writes to a file asynchronously
 *
 *	The file must have \p FILE_WRITE access.
 *
 *	\param	file	Handle of the file to read from
 *	\param	buf	Buffer to write out of
 *	\param	bytes	Number of bytes to write
 *	\param	op	\p fileop_t structure that receives the results of the write
 *	\return	\p true if the write could be started
 */
bool FsWrite(handle_t file, const void *buf, size_t bytes, struct fileop_t *op)
{
	return FsReadWrite(file, (void*) buf, bytes, op, false);
}

/*!
 *	\brief	Seeks to a location in a file
 *
 *	Subsequent reads and writes will take place starting at the location
 *	specified.
 *	\param	file	Handle to the file to seek
 *	\param	ofs	New offset, relative to the beginning of the file
 *	\return	New offset, or 0 if the handle was invalid
 */
addr_t FsSeek(handle_t file, addr_t ofs)
{
	file_t *fd;
	
	fd = HndLock(NULL, file, 'file');
	if (fd == NULL)
		return 0;

	fd->pos = ofs;
	HndUnlock(NULL, file, 'file');

	return ofs;
}

bool FsRequestSync(handle_t file, request_t *req)
{
	file_t *fd;
	device_t *fsd;
	/*handle_hdr_t *ptr;*/

	fd = HndLock(NULL, file, 'file');
	if (fd == NULL)
		return false;

	fsd = fd->fsd;
	HndUnlock(NULL, file, 'file');

	/*ptr = HndGetPtr(NULL, file, 'file');
	wprintf(L"FsRequestSync(%u): %p:%p at %S(%d)\n",
		file, fd, fd->fsd, ptr->file, ptr->line);*/
	return IoRequestSync(fsd, req);
}

static bool FsMountDevice(const wchar_t *path, device_t *dev)
{
	request_fs_t req;
	
	if (wcscmp(path, L"/") == 0)
	{
		IoCloseDevice(root);
		root = dev;
		return true;
	}
	else
	{
		req.header.code = FS_MOUNT;
		req.params.fs_mount.name = path;
		req.params.fs_mount.name_size = (wcslen(path) * sizeof(wchar_t)) + 1;
		req.params.fs_mount.fsd = dev;
		if (IoRequestSync(root, (request_t*) &req))
			return true;
		else
		{
			errno = req.header.result;
			return false;
		}
	}
}

/*!
 *	\brief	Mounts a file system in a directory
 *
 *	The target directory must support the \p FS_MOUNT requests. Virtual
 *	folders (e.g. \p / and \p /System) support this.
 *
 *	\param	path	Full path specification of the new mount point
 *	\param	filesys	Name of the file system driver to use
 *	\param	dev	Device for the file system to use
 *	\return	\p true if the file system was mounted
 */
bool FsMount(const wchar_t *path, const wchar_t *filesys, device_t *dev)
{
	driver_t *driver;
	device_t *fsd;
	
	driver = DevInstallNewDriver(filesys);
	if (driver == NULL)
	{
		wprintf(L"%s: unknown driver\n", filesys);
		return false;
	}

	if (driver->mount_fs == NULL)
	{
		DevUnloadDriver(driver);
		return false;
	}

	fsd = driver->mount_fs(driver, path, dev);
	if (fsd == NULL)
	{
		DevUnloadDriver(driver);
		return false;
	}

	assert(fsd->vtbl != NULL);
	assert(fsd->vtbl->request != NULL);
	if (!FsMountDevice(path, fsd))
	{
		IoCloseDevice(fsd);
		return false;
	}
	else
		return true;
}

static const device_vtbl_t vfs_vtbl =
{
	VfsRequest,
	NULL,
	VfsFinishIo,
};

/*!
 *	\brief	Creates a new virtual directory
 *
 *	This effectively creates a new virtual file system and mounts it at
 *	\p path.
 *	\param	path	Full path specification for the new directory
 *	\return	\p true if the directory was created
 */
bool FsCreateVirtualDir(const wchar_t *path)
{
	vfs_dir_t *dir;

	dir = malloc(sizeof(vfs_dir_t));
	if (dir == NULL)
		return NULL;

	memset(dir, 0, sizeof(vfs_dir_t));
	dir->dev.vtbl = &vfs_vtbl;

	if (!FsMountDevice(path, &dir->dev))
	{
		free(dir);
		return false;
	}

	return true;
}

/*!
 *	\brief	Initializes the filing system
 *
 *	This function is called to mount any system and boot file systems. It 
 *	will mount:
 *	- The ramdisk on \p /System/Boot
 *	- The port file system on \p /System/Ports
 *	- The device file system on \p /System/Devices
 *	- (\p /System/Devices/ide0a on \p /hd using \p fat )
 *	- \p /System/Devices/fdc0 on \p /hd using \p fat
 *
 *	\return	\p true if all devices were mounted correctly
 */
bool FsInit(void)
{
	bool b;

	FsCreateVirtualDir(L"/");
	FsCreateVirtualDir(L"/system");
	b = FsMount(SYS_BOOT, L"ram", NULL);
	assert(b || "Failed to mount ramdisk");
	b = FsMount(SYS_PORTS, L"portfs", NULL);
	assert(b || "Failed to mount ports");
	FsMount(SYS_DEVICES, L"devfs", NULL);
	assert(b || "Failed to mount devices");

	DevInstallDevice(L"ata", NULL, NULL);
	DevInstallDevice(L"fdc", L"fdc0", NULL);
	
	/*dev = DevOpen(L"ide0a");
	wprintf(L"FsInit: Mounting ide0a(%p) on /hd using fat\n", dev);
	FsMount(L"/hd", L"fat", dev);*/
	return true;
}
