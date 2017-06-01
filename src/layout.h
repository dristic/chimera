// Copyright 2017 Dan Ristic

#ifndef SRC_LAYOUT_H_
#define SRC_LAYOUT_H_

#include <vector>

#include "src/types.h"

namespace Chimera {

class Element;
class Style;

class Layout {
 public:
    Layout();

    void layout(Style& style);

    void updateChildren(std::vector<Element*>& children, Style& style);

    Rect rect;
    int intrinsicWidth{0};
    int intrinsicHeight{0};
};

}  // namespace Chimera

#endif  // SRC_LAYOUT_H_
