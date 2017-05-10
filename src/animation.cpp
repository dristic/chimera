// Copyright 2016 Dan Ristic

#include "src/animation.h"

#include "src/element.h"

namespace Nova {

    Animation::Animation(
            std::string _name,
            Element* element,
            std::function<void(float, Element*)> update)
        : name{_name}
        , mCurrent{0}
        , mEnd{1}
        , mElement{element}
        , mUpdate{update}
        { }

    Animation::~Animation() { }

    void Animation::update(double dt) {
        if (mCurrent < mEnd) {
            mUpdate(mCurrent, mElement);
            mCurrent += 0.08f;
        }
    }

    AnimationController::AnimationController()
        : mAnimations{}
        , mFunctions{}
        { }

    AnimationController::~AnimationController() { }

    void AnimationController::update(double dt) {
        for (auto &animation : mAnimations) {
            animation.update(dt);
        }
    }

    void AnimationController::addAnimation(
            Element* element, AnimationFunction update) {
        mAnimations.push_back({"", element, update});
    }

    void AnimationController::addAnimation(
            std::string name, AnimationFunction update) {
        mFunctions[name] = update;
    }

    AnimationFunction AnimationController::getFunction(std::string name) {
        if (mFunctions.count(name) == 1) {
            return mFunctions.at(name);
        }

        return nullptr;
    }

}  // namespace Nova
