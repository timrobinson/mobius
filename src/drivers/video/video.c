#include <kernel/kernel.h>
#include <kernel/driver.h>
#include <errno.h>
#include <wchar.h>
#include <limits.h>
#include <stdlib.h>

#include "video.h"

void swap_int(int *a, int *b)
{
	int temp = *b;
	*b = *a;
	*a = temp;
}

extern byte cupertino[];
vga_font_t _Cupertino={ 0, 256, 12, cupertino };

int *__errno()
{
	static int errno;
	return &errno;
}

typedef struct video_drv_t video_drv_t;
struct video_drv_t
{
	device_t dev;
	video_t *vid;
};

video_t *vga4Init();

struct
{
	const wchar_t *name;
	video_t *(*init)();
	video_t *vid;
} drivers[] =
{
	{ L"vga4",	vga4Init,	NULL },
	{ NULL,		NULL,		NULL },
};

typedef struct modemap_t modemap_t;
struct modemap_t
{
	videomode_t mode;
	video_t *vid;
};

modemap_t *modes;
unsigned numModes;

void vidHLine(video_t *vid, int x1, int x2, int y, colour_t c)
{
	for (; x1 < x2; x1++)
		vid->vidPutPixel(vid, x1, y, c);
}

void vidVLine(video_t *vid, int x, int y1, int y2, colour_t c)
{
	for (; y1 < y2; y1++)
		vid->vidPutPixel(vid, x, y1, c);
}

/* from Allegro gfx.c */
static void do_line(video_t *vid, int x1, int y1, int x2, int y2, colour_t d, 
					void (*proc)(video_t*, int, int, colour_t))
{
	int dx = x2-x1;
	int dy = y2-y1;
	int i1, i2;
	int x, y;
	int dd;
	
	/* worker macro */
#define DO_LINE(pri_sign, pri_c, pri_cond, sec_sign, sec_c, sec_cond)	\
	{																	\
		if (d##pri_c == 0) {											\
			proc(vid, x1, y1, d); 									\
			return;														\
		}																\
		\
		i1 = 2 * d##sec_c;												\
		dd = i1 - (sec_sign (pri_sign d##pri_c));						\
		i2 = dd - (sec_sign (pri_sign d##pri_c));						\
		\
		x = x1; 														\
		y = y1; 														\
		\
		while (pri_c pri_cond pri_c##2) {								\
				proc(vid, x, y, d);									\
			if (dd sec_cond 0) {										\
				sec_c sec_sign##= 1;									\
				dd += i2;												\
			}															\
			else														\
				dd += i1;												\
			\
			pri_c pri_sign##= 1;										\
		}																\
	}
	
	if (dx >= 0) {
		if (dy >= 0) {
			if (dx >= dy) {
				/* (x1 <= x2) && (y1 <= y2) && (dx >= dy) */
				DO_LINE(+, x, <=, +, y, >=);
			}
			else {
				/* (x1 <= x2) && (y1 <= y2) && (dx < dy) */
				DO_LINE(+, y, <=, +, x, >=);
			}
		}
		else {
			if (dx >= -dy) {
				/* (x1 <= x2) && (y1 > y2) && (dx >= dy) */
				DO_LINE(+, x, <=, -, y, <=);
			}
			else {
				/* (x1 <= x2) && (y1 > y2) && (dx < dy) */
				DO_LINE(-, y, >=, +, x, >=);
			}
		}
	}
	else {
		if (dy >= 0) {
			if (-dx >= dy) {
				/* (x1 > x2) && (y1 <= y2) && (dx >= dy) */
				DO_LINE(-, x, >=, +, y, >=);
			}
			else {
				/* (x1 > x2) && (y1 <= y2) && (dx < dy) */
				DO_LINE(+, y, <=, -, x, <=);
			}
		}
		else {
			if (-dx >= -dy) {
				/* (x1 > x2) && (y1 > y2) && (dx >= dy) */
				DO_LINE(-, x, >=, -, y, <=);
			}
			else {
				/* (x1 > x2) && (y1 > y2) && (dx < dy) */
				DO_LINE(-, y, >=, -, x, <=);
			}
		}
	}
}

void vidLine(video_t *vid, int x1, int y1, int x2, int y2, colour_t d)
{
	if (x1 == x2)
		vid->vidVLine(vid, x1, y1, y2, d);
	else if (y1 == y2)
		vid->vidHLine(vid, x1, x2, y1, d);
	else
		do_line(vid, x1, y1, x2, y2, d, vid->vidPutPixel);
}

void vidFillRect(video_t *vid, int x1, int y1, int x2, int y2, colour_t c)
{
	for (; y1 < y2; y1++)
		vid->vidHLine(vid, x1, x2, y1, c);
}

int vidMatchMode(videomode_t *a, videomode_t *b)
{
	if ((b->width == 0 || a->width == b->width) &&
		(b->height == 0 || a->height == b->height) &&
		(b->bitsPerPixel == 0 || a->bitsPerPixel == b->bitsPerPixel))
		return 0;
	else
		return INT_MAX;
}

bool vidSetMode(video_drv_t *video, videomode_t *mode)
{
	int i, best, score, s;
	video_t *vid;

	if (video->vid)
	{
		video->vid->vidClose(video->vid);
		video->vid = NULL;
	}

	best = -1;
	score = INT_MAX;
	for (i = 0; i < numModes; i++)
	{
		s = vidMatchMode(&modes[i].mode, mode);
		if (s < score)
		{
			best = i;
			score = s;
		}
	}

	if (best == -1)
	{
		errno = ENOTFOUND;
		return false;
	}

	*mode = modes[best].mode;
	vid = modes[best].vid;

	assert(vid->vidClose != NULL);
	assert(vid->vidEnumModes != NULL);
	assert(vid->vidSetMode != NULL);
	assert(vid->vidPutPixel != NULL);

	if (vid->vidHLine == NULL)
		vid->vidHLine = vidHLine;
	if (vid->vidVLine == NULL)
		vid->vidVLine = vidVLine;
	if (vid->vidFillRect == NULL)
		vid->vidFillRect = vidFillRect;
	if (vid->vidLine == NULL)
		vid->vidLine = vidLine;
	
	if (!vid->vidSetMode(vid, mode))
		return false;

	video->vid = vid;
	wprintf(L"video: using mode %ux%ux%u\n", 
		mode->width, mode->height, mode->bitsPerPixel);

	return true;
}

#define VID_OP(code, type) \
	case code: \
	{ \
		type *shape; \
		shape = (type*) &buf->s; \
		\
		
#define VID_END_OP \
		break; \
	}

bool vidRequest(device_t* dev, request_t* req)
{
	video_drv_t *video = (video_drv_t*) dev;
	
	switch (req->code)
	{
	case DEV_REMOVE:
		hndFree(dev);

	case DEV_OPEN:
	case DEV_CLOSE:
		hndSignal(req->event, true);
		return true;

	case VID_SETMODE:
		if (!vidSetMode(video, (videomode_t*) &req->params))
		{
			req->result = errno;
			return false;
		}
		else
		{
			hndSignal(req->event, true);
			return true;
		}

	case VID_DRAW:
		{
			vid_shape_t *buf;

			req->user_length = req->params.buffered.length;
			req->params.buffered.length = 0;
			buf = (vid_shape_t*) req->params.buffered.buffer;

			while (req->params.buffered.length < req->user_length)
			{
				switch (buf->shape)
				{
				VID_OP(VID_SHAPE_FILLRECT, vid_rect_t)
					video->vid->vidFillRect(video->vid, 
						shape->rect.left, shape->rect.top, 
						shape->rect.right, shape->rect.bottom, 
						shape->colour);
				VID_END_OP

				VID_OP(VID_SHAPE_HLINE, vid_line_t)
					if (shape->a.x != shape->b.x)
					{
						if (shape->b.x < shape->a.x)
							swap_int(&shape->a.x, &shape->b.x);
						video->vid->vidHLine(video->vid, 
							shape->a.x, shape->b.x, 
							shape->a.y, 
							shape->colour);
					}
				VID_END_OP

				VID_OP(VID_SHAPE_VLINE, vid_line_t)
					if (shape->a.y != shape->b.y)
					{
						if (shape->b.y < shape->a.y)
							swap_int(&shape->a.y, &shape->b.y);
						video->vid->vidVLine(video->vid, 
							shape->a.x, 
							shape->a.y, shape->b.y, 
							shape->colour);
					}
				VID_END_OP

				VID_OP(VID_SHAPE_LINE, vid_line_t)
					if (shape->b.x < shape->a.x)
						swap_int(&shape->a.x, &shape->b.x);
					if (shape->b.y < shape->a.y)
						swap_int(&shape->a.y, &shape->b.y);

					video->vid->vidLine(video->vid, 
						shape->a.x, shape->a.y, 
						shape->b.x, shape->b.y, 
						shape->colour);
				VID_END_OP

				VID_OP(VID_SHAPE_PUTPIXEL, vid_pixel_t)
					video->vid->vidPutPixel(video->vid, 
						shape->point.x, shape->point.y,
						shape->colour);
				VID_END_OP

				VID_OP(VID_SHAPE_GETPIXEL, vid_pixel_t)
					shape->colour = video->vid->vidGetPixel(video->vid, 
						shape->point.x, shape->point.y);
				VID_END_OP
				}

				buf++;
				req->params.buffered.length += sizeof(*buf);
			}

			hndSignal(req->event, true);
			return true;
		}

	case VID_TEXTOUT:
	{
		vid_text_t *p = (vid_text_t*) &req->params;
		video->vid->vidTextOut(video->vid, p->x, p->y, &_Cupertino, 
			(const wchar_t*) p->buffer, p->length / sizeof(wchar_t), 
			p->foreColour, p->backColour);
		hndSignal(req->event, true);
		return true;
	}

	case VID_STOREPALETTE:
	{
		vid_palette_t *p = (vid_palette_t*) &req->params;
		if (video->vid->vidStorePalette)
		{
			video->vid->vidStorePalette(video->vid, 
				(const rgb_t*) p->entries,
				p->first_index,
				p->length / sizeof(rgb_t));
			hndSignal(req->event, true);
			return true;
		}
		break;
	}
	}

	req->result = ENOTIMPL;
	return false;
}

device_t* vidAddDevice(driver_t* drv, const wchar_t* name, device_config_t* cfg)
{
	video_drv_t* dev;
	int i, j, code;
	videomode_t mode;
	video_t *vid;

	for (i = 0; drivers[i].name; i++)
	{
		vid = drivers[i].vid = drivers[i].init();

		j = 0;
		do
		{
			code = vid->vidEnumModes(vid, j, &mode);
			numModes++;
			modes = realloc(modes, sizeof(modemap_t) * numModes);
			modes[numModes - 1].vid = vid;
			modes[numModes - 1].mode = mode;
			j++;
		} while (code != VID_ENUM_STOP);
	}

	dev = hndAlloc(sizeof(video_drv_t), NULL);
	dev->dev.request = vidRequest;
	dev->dev.driver = drv;
	dev->vid = NULL;

	if (numModes > 0)
		vidSetMode(dev, &modes[0].mode);
	return &dev->dev;
}

bool STDCALL INIT_CODE drvInit(driver_t* drv)
{
	drv->add_device = vidAddDevice;
	return true;
}