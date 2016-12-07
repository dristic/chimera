// Copyright 2016 Dan Ristic

#ifndef SRC_LAYOUT_H_
#define SRC_LAYOUT_H_

#include "src/style.h"
#include "src/types.h"

namespace Nova {

class Layout {
 public:
    Layout();

    void layout(Style& style);

    Rect rect;
};

}  // namespace Nova

#endif  // SRC_LAYOUT_H_
