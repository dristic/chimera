// Copyright 2017 Dan Ristic

#ifndef CHIMERA_ADAPTOR_ADAPTOR
#define CHIMERA_ADAPTOR_ADAPTOR

#include <string>
#include <memory>

#include "chimera/core.h"
#include "chimera/rendering.h"

namespace Chimera {

class Adaptor {
 public:
    Adaptor() { }

    virtual void renderCallback(DrawData* data)
    {
        CHIMERA_UNUSED(data);
    }

    virtual std::unique_ptr<ImageRef> loadImage(
        std::string name, int& width, int& height)
    {
        CHIMERA_UNUSED(name);
        CHIMERA_UNUSED(width);
        CHIMERA_UNUSED(height);

        return nullptr;
    }

    virtual unsigned int loadTexture(
        unsigned int width, unsigned int height, unsigned char* buffer)
    {
        CHIMERA_UNUSED(width);
        CHIMERA_UNUSED(height);
        CHIMERA_UNUSED(buffer);

        return 0;
    }
};

}  // namespace Chimera

#endif  // CHIMERA_ADAPTOR_ADAPTOR