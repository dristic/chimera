
#include "src/adaptor/dx9.h"

namespace Cosmonaut {

DX9Adaptor::DX9Adaptor() {

}

unsigned int DX9Adaptor::loadTexture(unsigned int width, unsigned int height, unsigned char* buffer) {
    textures.push_back({});

    auto result = device->CreateTexture(
        width,
        height,
        1,
        0,
        D3DFMT_A8R8G8B8,
        D3DPOOL_MANAGED,
        &textures[textureId],
        nullptr);

    if (result != D3D_OK) {
        char buffer[100];
        sprintf_s(buffer, "Failed to create texture of %u %u.\n", width, height);
        OutputDebugString(TEXT(buffer));

        return 0;
    }

    D3DLOCKED_RECT lockedRect;
    if (textures[textureId]->LockRect(0, &lockedRect, nullptr, 0) != D3D_OK) {
        OutputDebugString(TEXT("Failed to lock texture.\n"));

        return 0;
    }

    for (int y = 0; y < height; y++) {
        memcpy(
            static_cast<unsigned char *>(lockedRect.pBits) + lockedRect.Pitch * y,
            buffer + (width * 4) * y,
            (width * 4)
        );
    }

    textures[textureId]->UnlockRect(0);

    device->SetTexture(0, textures[textureId]);

    auto returnId = textureId;
    textureId++;

    return returnId;
}

void DX9Adaptor::renderCallback(Nova::DrawData* data) {
    device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    device->SetRenderState(D3DRS_LIGHTING, false);
    device->SetRenderState(D3DRS_ZENABLE, false);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
    device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
    device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    if (textureId > 0) device->SetTexture(0, textures[textures.size() - 1]);
}

}  // namespace Cosmonaut
