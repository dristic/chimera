// Copyright 2017 Dan Ristic

#include "src/layout.h"

namespace Chimera {

Layout::Layout()
    : rect{}
    { }

void Layout::layout(Style& style)
{
    rect.width = style.width;
    rect.height = style.height;

    if (style.position == Position::Absolute)
    {
        rect.x = style.left;
        rect.y = style.top;
    }

    if (rect.width < intrinsicWidth)
    {
        rect.width = intrinsicWidth;
    }

    if (rect.height < intrinsicHeight)
    {
        rect.height = intrinsicHeight;
    }

    if (rect.height < style.fontSize)
    {
        rect.height = style.fontSize;
    }
}

}  // namespace Chimera
