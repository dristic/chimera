// Copyright 2016 Dan Ristic

#ifndef SRC_ADAPTOR_DX9
#define SRC_ADAPTOR_DX9

#include "src/adaptor/adaptor.h"
#include "src/context.h"
#include "src/rendering.h"
#include "src/types.h"

#include <vector>

#include <Windows.h>
#include <d3d9.h>

struct CUSTOMVERTEX {
    FLOAT X, Y, Z;
    DWORD COLOR;
    FLOAT U, V;
};
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

namespace Cosmonaut {

class DX9Adaptor : public Cosmonaut::Adaptor {
public:
    DX9Adaptor(LPDIRECT3DDEVICE9 _device);

    std::unique_ptr<Cosmonaut::ImageRef> loadImage(std::string imagePath, int& width, int& height) override;
    unsigned int loadTexture(unsigned int width, unsigned int height, unsigned char* buffer) override;
    void renderCallback(Cosmonaut::DrawData* data) override;

    LPDIRECT3DDEVICE9 device;

    unsigned int textureId{ 1 };
    std::vector<LPDIRECT3DTEXTURE9> textures;
    LPDIRECT3DINDEXBUFFER9 iBuffer{ nullptr };
    LPDIRECT3DVERTEXBUFFER9 vBuffer{ nullptr };
};

}  // namespace Cosmonaut

#endif  // SRC_ADAPTOR_DX9