// Copyright 2017 Dan Ristic

#ifndef SRC_ADAPTOR_ADAPTOR
#define SRC_ADAPTOR_ADAPTOR

#include <string>
#include <memory>

#include "src/rendering.h"

namespace Nova {

class Adaptor {
 public:
    Adaptor() { }

    virtual void renderCallback(DrawData* data) { }
    virtual std::unique_ptr<ImageRef> loadImage(std::string name, int& width, int& height) {
        return nullptr;
    }
    virtual unsigned int loadTexture(unsigned int width, unsigned int height, unsigned char* buffer) {
        return 0;
    }
};

}  // namespace Nova

#endif  // SRC_ADAPTOR_ADAPTOR