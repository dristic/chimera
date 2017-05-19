// Copyright 2017 Dan Ristic

#include "src/focus.h"

namespace Cosmonaut {

FocusManager::FocusManager()
    : focusedElement{}
    { }

FocusManager::~FocusManager() { }

void FocusManager::focusElement(Element* element) {
    focusedElement = element;
}

}  // namespace Cosmonaut
