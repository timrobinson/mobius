/* $Id: label.h,v 1.1 2002/04/03 23:29:31 pavlovskii Exp $ */

#ifndef __GUI_LABEL_H
#define __GUI_LABEL_H

#include <gui/control.h>

namespace os
{
    //! Label control
    /*! \ingroup gui */
    class Label : public Control
    {
    public:
        Label(Window *parent, const wchar_t *text, const MGLrect &pos);
        void OnPaint();
    };
};

#endif