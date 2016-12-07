// Copyright 2016 Dan Ristic

#ifndef SRC_FOCUS_H_
#define SRC_FOCUS_H_

#include "src/element.h"

namespace Nova {

class FocusManager {
 public:
    FocusManager();
    ~FocusManager();

    void focusElement(Element* element);

    Element* focusedElement;
};

}  // namespace Nova

#endif  // SRC_FOCUS_H_
