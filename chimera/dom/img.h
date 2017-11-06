// Copyright 2017 Dan Ristic

#ifndef CHIMERA_DOM_IMG_H_
#define CHIMERA_DOM_IMG_H_

#include "chimera/element.h"

namespace Chimera {

class Img: public Element {
 public:
    explicit Img(Document& document);
    ~Img();

    void render(DrawData* data);

    std::string const& getSrc() const { return mSrc; }
    void setSrc(std::string const& newSrc);

 private:
    std::string mSrc;
    std::unique_ptr<ImageRef> mImageRef;
};

}  // namespace Chimera

#endif  // CHIMERA_DOM_IMG_H_