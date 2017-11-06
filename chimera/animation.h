// Copyright 2017 Dan Ristic

#ifndef CHIMERA_ANIMATION_H_
#define CHIMERA_ANIMATION_H_

#include "chimera/animation.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

namespace Chimera {
    
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

}  // namespace Chimera

#endif  // CHIMERA_ANIMATION_H_
