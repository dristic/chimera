
#include "src/adaptor/dx9.h"

#include "lodepng/lodepng.h"

namespace Cosmonaut {

DX9Adaptor::DX9Adaptor(LPDIRECT3DDEVICE9 _device)
    : device{ _device }
    { }

std::unique_ptr<Nova::ImageRef> DX9Adaptor::loadImage(std::string imagePath, int& width, int& height) {
    std::vector<unsigned char> image;  // The raw pixels
    unsigned w, h;
    unsigned error = lodepng::decode(image, w, h, imagePath.c_str());

    // If there is an error, display it
    if (error) {
        char buffer[100];
        sprintf_s(buffer, "Failed to decode png with error: %s\n", lodepng_error_text(error));
        OutputDebugString(TEXT(buffer));
        throw std::runtime_error("Failed to decode png");
    }

    width = static_cast<int>(w);
    height = static_cast<int>(h);

    for (int i = 0; i < image.size(); i += 4) {
        unsigned char r = image[i + 0];
        unsigned char g = image[i + 1];
        unsigned char b = image[i + 2];
        unsigned char a = image[i + 3];
        image[i + 0] = b;
        image[i + 1] = g;
        image[i + 2] = r;
        image[i + 3] = a;
    }

    auto textureID = loadTexture(width, height, reinterpret_cast<unsigned char*>(image.data()));

    return std::make_unique<Nova::ImageRef>(width, height, textureID);
}

unsigned int DX9Adaptor::loadTexture(unsigned int width, unsigned int height, unsigned char* buffer) {
    if (textures.size() <= textureId) {
        textures.resize(textures.size() + 100);
    }

    auto result = device->CreateTexture(
        width,
        height,
        1,
        D3DUSAGE_DYNAMIC,
        D3DFMT_A8R8G8B8,
        D3DPOOL_DEFAULT,
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
    device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

    D3DVIEWPORT9 vp;
    vp.X = 0;
    vp.Y = 0;
    vp.Width = data->screenWidth;
    vp.Height = data->screenHeight;
    vp.MinZ = 0.0f;
    vp.MaxZ = 1.0f;
    device->SetViewport(&vp);

    float scale = data->screenWidth / data->width;

    D3DMATRIX identityMatrix = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    D3DMATRIX orthoMatrix = {
        2.0f / data->width, 0.0f, 0.0f,  0.0f,
        0.0f, 2.0f / -data->height, 0.0f, 0.0f,
        0.0f, 0.0f, -1.0f,  0.0f,
        -1.0f, 1.0f, 0.0f,  1.0f
    };
    device->SetTransform(D3DTS_WORLD, &identityMatrix);
    device->SetTransform(D3DTS_VIEW, &identityMatrix);
    device->SetTransform(D3DTS_PROJECTION, &orthoMatrix);

    // Fill vertex buffer
    if (vBuffer) {
        vBuffer->Release();
        vBuffer = nullptr;
    }
    device->CreateVertexBuffer(sizeof(CUSTOMVERTEX) * data->vertices.size(),
        0,
        CUSTOMFVF,
        D3DPOOL_DEFAULT,
        &vBuffer,
        nullptr);
    CUSTOMVERTEX* vertexData;
    vBuffer->Lock(0, 0, reinterpret_cast<void**>(&vertexData), 0);
    for (auto& vert : data->vertices) {
        vertexData->X = vert.pos.x;
        vertexData->Y = vert.pos.y;
        vertexData->Z = 0.0f;
        vertexData->COLOR = D3DCOLOR_ARGB(
            static_cast<int>(vert.color.w * 255.0f),
            static_cast<int>(vert.color.x * 255.0f),
            static_cast<int>(vert.color.y * 255.0f),
            static_cast<int>(vert.color.z * 255.0f)
        );
        vertexData->U = vert.uv.x;
        vertexData->V = vert.uv.y;
        vertexData++;
    }
    vBuffer->Unlock();

    // Fill index buffer
    if (iBuffer) {
        iBuffer->Release();
        iBuffer = nullptr;
    }
    device->CreateIndexBuffer(data->indicies.size() * sizeof(unsigned int),
        0,
        D3DFMT_INDEX32,
        D3DPOOL_DEFAULT,
        &iBuffer,
        nullptr);
    void* indexData;
    iBuffer->Lock(0, sizeof(unsigned int) * data->indicies.size(), &indexData, 0);
    memcpy(indexData, data->indicies.data(), sizeof(unsigned int) * data->indicies.size());
    iBuffer->Unlock();

    device->SetFVF(CUSTOMFVF);
    device->SetStreamSource(0, vBuffer, 0, sizeof(CUSTOMVERTEX));
    device->SetIndices(iBuffer);

    unsigned int offset = 0;
    for (auto &command : data->commands) {
        unsigned int textureId = command.textureId;

        device->SetTexture(0, textures[textureId]);

        int sy = data->height - (command.scissor.y + command.scissor.height);

        RECT scissorRect = {
            command.scissor.x * scale,
            sy * scale,
            command.scissor.width * scale,
            command.scissor.height *  scale
        };
        //device->SetScissorRect(&scissorRect);

        device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
            0,                  // BaseVertexIndex
            0,                  // MinIndex
            data->vertices.size(),                  // NumVertices
            offset,                  // StartIndex
            command.elements / 3);                // PrimitiveCount

        offset += command.elements;
    }
}

}  // namespace Cosmonaut
