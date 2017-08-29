// Copyright 2017 Dan Ristic

#include "src/dom/img.h"

#include "src/document.h"
#include "src/rendering.h"

namespace Chimera {

Img::Img(Document& document)
    : Element("img", document)
    , mSrc{""}
    , mImageRef{nullptr}
    { }

Img::~Img() { }

void Img::render(DrawData* data) {
    layout.intrinsicWidth = style.width;
    layout.intrinsicHeight = style.height;

    layout.layout();

    if (mImageRef != nullptr) {
        mImageRef->render();
        data->addImage(layout.rect, mImageRef->textureId, style.opacity);
    }

    Element::render(data);
}

void Img::setSrc(std::string const& newSrc) {
    mSrc = newSrc;
    mImageRef = mDocument->loadImage(mSrc, style.width, style.height);
}

}  // namespace Chimera