// Copyright 2016 Dan Ristic

#include "src/focus.h"

namespace Nova {

FocusManager::FocusManager()
    : focusedElement{}
    { }

FocusManager::~FocusManager() { }

void FocusManager::focusElement(Element* element) {
    focusedElement = element;
}

}  // namespace Nova
