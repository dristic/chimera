// Copyright 2017 Dan Ristic

#ifndef SRC_DOM_IMG_H_
#define SRC_DOM_IMG_H_

#include "src/element.h"

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

#endif  // SRC_DOM_IMG_H_