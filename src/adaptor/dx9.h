// Copyright 2016 Dan Ristic

#ifndef SRC_ADAPTOR_DX9
#define SRC_ADAPTOR_DX9

#include <src/adaptor/adaptor.h>
#include <src/rendering.h>

#include <vector>

#include <Windows.h>
#include <d3d9.h>

namespace Cosmonaut {

class DX9Adaptor : public Nova::Adaptor {
public:
    DX9Adaptor();

    unsigned int loadTexture(unsigned int width, unsigned int height, unsigned char* buffer) override;
    void renderCallback(Nova::DrawData* data) override;

    LPDIRECT3DDEVICE9 device;

    unsigned int textureId{ 0 };
    std::vector<LPDIRECT3DTEXTURE9> textures;
};

}  // namespace Cosmonaut

#endif  // SRC_ADAPTOR_DX9