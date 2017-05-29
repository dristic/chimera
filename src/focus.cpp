// Copyright 2017 Dan Ristic

#include "src/focus.h"

namespace Chimera {

FocusManager::FocusManager()
    : focusedElement{}
    { }

FocusManager::~FocusManager() { }

void FocusManager::focusElement(Element* element) {
    focusedElement = element;
}

}  // namespace Chimera
