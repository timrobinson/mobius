/* $Id: render.h,v 1.2 2002/03/06 19:31:41 pavlovskii Exp $ */

#ifndef __INTERNAL_RENDER_H
#define __INTERNAL_RENDER_H

bool	vidFlushQueue(queue_t *queue, handle_t video);

bool	vidFillRect(queue_t *queue, point_t topLeft, point_t bottomRight, 
					colour_t colour);
bool	vidVLine(queue_t *queue, int x, int y1, int y2, colour_t colour);
bool	vidHLine(queue_t *queue, int x1, int x2, int y, colour_t colour);
bool	vidLine(queue_t *queue, point_t from, point_t to, colour_t colour);
bool	vidPutPixel(queue_t *queue, int x, int y, colour_t colour);

#endif