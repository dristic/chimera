// Copyright 2016 Dan Ristic

#ifndef SRC_ANIMATION_H_
#define SRC_ANIMATION_H_

#include "src/animation.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

namespace Cosmonaut {
    
class Element;

using AnimationFunction = std::function<void(float, Element*)>;

class Animation {
 public:
    Animation(std::string _name, Element* element, std::function<void(float, Element*)> update);
    ~Animation();

    void update(double dt);

    std::string name;

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
    void addAnimation(Element* element, AnimationFunction update);

    void addAnimation(std::string name, AnimationFunction update);

    AnimationFunction getFunction(std::string name);

 private:
    std::vector<Animation> mAnimations;
    std::unordered_map<std::string, AnimationFunction> mFunctions;
};

}  // namespace Cosmonaut

#endif  // SRC_ANIMATION_H_
