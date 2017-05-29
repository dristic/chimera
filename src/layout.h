// Copyright 2017 Dan Ristic

#ifndef SRC_LAYOUT_H_
#define SRC_LAYOUT_H_

#include "src/style.h"
#include "src/types.h"

namespace Chimera {

class Layout {
 public:
    Layout();

    void layout(Style& style);

    Rect rect;
};

}  // namespace Chimera

#endif  // SRC_LAYOUT_H_
