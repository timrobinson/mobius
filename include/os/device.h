/* $Id: device.h,v 1.9 2002/02/20 01:35:52 pavlovskii Exp $ */
#ifndef __OS_DEVICE_H
#define __OS_DEVICE_H

#include <sys/types.h>

/*!
 *	\ingroup	libsys
 *	\defgroup	osdev	Device Interface
 *	@{
 */

typedef union params_dev_t params_dev_t;

/*!	\brief	Parameters for a \p DEV_xxx request */
union params_dev_t
{
	struct
	{
		uint32_t length;
		void *buffer;
		uint64_t offset;
	} buffered;

	struct
	{
		/*! Number of bytes to read */
		uint32_t length;
		/*! Buffer into which to read */
		void *buffer;
		/*!
		 *	\brief	Offset of the first byte to read
		 *
		 *	Block devices (e.g. disk drives) must honour this; character 
		 *	devices (e.g. serial ports) may ignore this.
		 */
		uint64_t offset;
	} dev_read;

	struct
	{
		/*! Number of bytes to write */
		uint32_t length;
		/*! Buffer from which to write */
		const void *buffer;
		/*!
		 *	\brief	Offset of the first byte to write
		 *
		 *	Block devices (e.g. disk drives) must honour this; character 
		 *	devices (e.g. serial ports) may ignore this.
		 */
		uint64_t offset;
	} dev_write;

	struct
	{
		uint32_t size;
		void *params;
		uint64_t unused;
	} dev_ioctl;

	struct
	{
		uint8_t irq;
	} dev_irq;
};

typedef union params_fs_t params_fs_t;
/*!	\brief	Parameters for a \p FS_xxx request */
union params_fs_t
{
	struct
	{
		size_t length;
		void *buffer;
		handle_t file;
	} buffered;

	struct
	{
		size_t length;
		void *buffer;
		handle_t file;
	} fs_read;

	struct
	{
		size_t length;
		const void *buffer;
		handle_t file;
	} fs_write;

	struct
	{
		size_t length;
		void *buffer;
		handle_t file;
		uint32_t code;
	} fs_ioctl;

	struct
	{
		size_t name_size;
		const wchar_t *name;
		handle_t file;
		uint32_t flags;
	} fs_open, fs_create;

	struct
	{
		handle_t file;
	} fs_close;

	struct
	{
		handle_t file;
		uint64_t length;
	} fs_getlength;

	struct
	{
		size_t name_size;
		const wchar_t *name;
		struct device_t *fsd;
	} fs_mount;
};

typedef union params_port_t params_port_t;
/*!	\brief	Parameters for a \p PORT_xxx request */
union params_port_t
{
	struct
	{
		uint32_t name_size;
		const wchar_t *remote;
		handle_t port;
	} port_connect;

	struct
	{
		uint32_t unused[2];
		handle_t port;
	} port_listen;

	struct
	{
		uint32_t unused[2];
		handle_t port;
		handle_t client;
		uint32_t flags;
	} port_accept;
};

#define REQUEST_CODE(buff, sys, maj, min)	\
	((uint32_t) ((buff) << 31 | \
	((sys) << 16) | \
	((maj) << 8) | (min)))

#define	DEV_OPEN		REQUEST_CODE(0, 0, 'd', 'o')
#define	DEV_CLOSE		REQUEST_CODE(0, 0, 'd', 'c')
#define DEV_REMOVE		REQUEST_CODE(0, 0, 'd', 'R')
#define	DEV_READ		REQUEST_CODE(1, 0, 'd', 'r')
#define	DEV_WRITE		REQUEST_CODE(1, 0, 'd', 'w')
#define	DEV_IOCTL		REQUEST_CODE(1, 0, 'd', 'i')

#define FS_OPEN			REQUEST_CODE(1, 0, 'f', 'o')
#define FS_CREATE		REQUEST_CODE(1, 0, 'f', 'C')
#define FS_CLOSE		REQUEST_CODE(0, 0, 'f', 'c')
#define FS_MOUNT		REQUEST_CODE(1, 0, 'f', 'm')
#define FS_READ			REQUEST_CODE(1, 0, 'f', 'r')
#define FS_WRITE		REQUEST_CODE(1, 0, 'f', 'w')
#define FS_IOCTL		REQUEST_CODE(1, 0, 'f', 'i')
#define FS_GETLENGTH	REQUEST_CODE(0, 0, 'f', 'l')

#define BLK_GETSIZE		REQUEST_CODE(1, 0, 'b', 's')

#define CHR_GETSIZE		REQUEST_CODE(1, 0, 'c', 's')

/* Ports respond to the FS_xxx codes as well */
#define PORT_CONNECT	REQUEST_CODE(1, 0, 'p', 'c')
#define PORT_LISTEN		REQUEST_CODE(0, 0, 'p', 'l')
#define PORT_ACCEPT		REQUEST_CODE(0, 0, 'p', 'a')

/*! @} */

#endif