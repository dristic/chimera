// Copyright 2016 Dan Ristic

#ifndef SRC_ANIMATION_H_
#define SRC_ANIMATION_H_

#include "src/animation.h"

#include <vector>
#include <functional>

namespace Nova {
    
class Element;

class Animation {
 public:
    Animation(Element* element, std::function<void(float, Element*)> update);
    ~Animation();

    void update(double dt);

 private:
    float mCurrent;
    float mEnd;
    Element* mElement;
    std::function<void(float, Element*)> mUpdate;
};

class AnimationController {
 public:
    AnimationController();
    ~AnimationController();

    void update(double dt);
    void addAnimation(
        Element* element, std::function<void(float, Element*)> update);

 private:
    std::vector<Animation> mAnimations;
};

}  // namespace Nova

#endif  // SRC_ANIMATION_H_
