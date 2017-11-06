// Copyright 2017 Dan Ristic

#include "chimera/layout.h"

#include <algorithm>

#include "chimera/style/style.h"
#include "chimera/element.h"

namespace Chimera {

Layout::Layout(Style* style)
    : rect{}
    , mStyle{style}
    { }

int Layout::getBoundingWidth()
{
    return int(rect.width + mStyle->margin.left + mStyle->margin.right);
}

int Layout::getBoundingHeight()
{
    return int(rect.height + mStyle->margin.top + mStyle->margin.bottom);
}

void Layout::calculateDimensions(const std::vector<Element*>& children)
{
    intrinsicWidth = 0;
    intrinsicHeight = 0;

    for (auto& child : children) {
        if (mStyle->flexDirection == Direction::Row)
        {
            intrinsicHeight = std::max(intrinsicHeight, child->layout.getBoundingHeight());
            intrinsicWidth += child->layout.getBoundingWidth();
        }
        else
        {
            intrinsicHeight += child->layout.getBoundingHeight();
            intrinsicWidth = std::max(intrinsicWidth, child->layout.getBoundingWidth());
        }
    }
}

void Layout::layout()
{
    rect.x += mStyle->margin.left;
    rect.y += mStyle->margin.top;

    rect.width = float(mStyle->width);
    rect.height = float(mStyle->height);

    if (mStyle->position == Position::Absolute)
    {
        rect.x = float(mStyle->left);
        rect.y = float(mStyle->top);
    }

    if (rect.width < intrinsicWidth)
    {
        rect.width = float(intrinsicWidth);
    }

    if (rect.height < intrinsicHeight)
    {
        rect.height = float(intrinsicHeight);
    }

    rect.width += mStyle->padding.left + mStyle->padding.right;
    rect.height += mStyle->padding.top + mStyle->padding.bottom;
}

void Layout::updateChildren(const std::vector<Element*>& children)
{
    float xValue = rect.x + mStyle->padding.left;
    float yValue = rect.y + mStyle->padding.top;

    for (auto &element : children) {
        if (element->style.position != Position::Absolute) {
            if (mStyle->justifyContent == Align::Center) {
                float midpoint = (rect.width - element->layout.rect.width) / 2;
                element->layout.rect.x = xValue + midpoint;
            } else if (mStyle->justifyContent == Align::Right) {
                element->layout.rect.x = xValue + rect.width - element->layout.rect.width;
            } else {
                element->layout.rect.x = xValue;
            }

            if (mStyle->alignItems == Align::Center) {
                float midpoint = (rect.height - element->layout.rect.height) / 2;
                element->layout.rect.y = yValue + midpoint;
            } else {
                element->layout.rect.y = yValue;
            }

            if (mStyle->flexDirection == Direction::Row) {
                xValue += element->layout.getBoundingWidth();
            } else {
                yValue += element->layout.getBoundingHeight();
            }
        }
    }
}

}  // namespace Chimera
