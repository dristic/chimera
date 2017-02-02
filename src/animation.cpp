// Copyright 2016 Dan Ristic

#include "src/animation.h"

#include "src/element.h"

namespace Nova {

    Animation::Animation(
            Element* element,
            std::function<void(float, Element*)> update)
        : mCurrent{0}
        , mEnd{1}
        , mElement{element}
        , mUpdate{update}
        { }

    Animation::~Animation() { }

    void Animation::update(double dt) {
        if (mCurrent < mEnd) {
            mUpdate(mCurrent, mElement);
            mCurrent += 0.08;
        }
    }

    AnimationController::AnimationController()
        : mAnimations{}
        { }

    AnimationController::~AnimationController() { }

    void AnimationController::update(double dt) {
        for (auto &animation : mAnimations) {
            animation.update(dt);
        }
    }

    void AnimationController::addAnimation(
            Element* element, std::function<void(float, Element*)> update) {
        mAnimations.push_back({element, update});
    }

}  // namespace Nova
