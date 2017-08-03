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
    explicit Layout(Style* style);

    void layout();

    int getBoundingWidth();
    int getBoundingHeight();

    void calculateDimensions(const std::vector<Element*>& children);
    void updateChildren(const std::vector<Element*>& children);

    Rect rect;
    int intrinsicWidth{0};
    int intrinsicHeight{0};

 private:
    Style* mStyle{nullptr};
};

}  // namespace Chimera

#endif  // SRC_LAYOUT_H_
