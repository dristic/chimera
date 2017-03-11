// Copyright 2016 Dan Ristic

#include "login.h"

#include "src/document.h"
#include "src/types.h"
#include "src/style.h"
#include "src/context.h"

void LoginComponent::onClick(Nova::Event *event) {
    printf("::: ON CLICK\n");
}

std::shared_ptr<Cosmonaut::Api::CElement> LoginComponent::render(Nova::Context &context) {
    using namespace Nova;
    using namespace Cosmonaut::Api;
    using namespace std::placeholders;

    Nova::Document& document = context.document;

    document.styleManager.addRule([this](StyleRule& rule) {
        rule.addId("login-container");
        rule.withWidth(mWidth / 1.5)
            .withHeight(mHeight / 1.5)
            .withJustifyContent(Align::Center)
            .withFlexDirection(Direction::Column)
            .withMargin({200, 0, 0, 0})
            .withAnimationName("slide-in");
    });

    document.styleManager.addRule([](StyleRule& rule) {
        rule.addId("title");
        rule.withWidth(256)
            .withHeight(40)
            .withColor(Color::fromRGBA(255, 255, 255, 1))
            .withTextAlign(Align::Center)
            .withFontSize(32)
            .withMargin({10, 0, 0, 0})
            .withAnimationName("fade-in");
    });

    document.styleManager.addRule([](StyleRule& rule) {
        rule.addId("logo");
        rule.withWidth(128)
            .withHeight(128)
            .withAnimationName("fade-in");
    });

    document.styleManager.addRule([](StyleRule& rule) {
        rule.addId("login-button");
        rule.withBackgroundColor(Color::fromRGBA(11, 37, 63, 1.0f))
            .withWidth(200)
            .withHeight(40)
            .withPadding({5, 0, 0, 0})
            .withColor({1.0f, 1.0f, 1.0f, 1.0f})
            .withTextAlign(Nova::Align::Center)
            .withAnimationName("fade-in");
    });

    document.styleManager.addRule([](StyleRule& rule) {
        rule.addId("username");
        rule.withBackgroundColor(Color::fromRGBA(150, 150, 150, 0.3))
            .withWidth(200)
            .withHeight(40)
            .withMargin({75, 0, 0, 15})
            .withColor({1.0f, 1.0f, 1.0f, 1.0f})
            .withAnimationName("fade-in");
    });

    return C(E::Div, {{"id", "login-container"}}, {
            C(E::Img, {{"id", "logo"}, {"src", "assets/outerspace.png"}}),
            C(E::Div, {{"id", "title"}}, {
                C(E::Text, {{"textContent", "Cosmonaut"}})
            }),
            C(E::Input, {{"id", "username"}, {"placeholder", "Username"}}),
            C(E::Button, {
                {"id", "login-button"},
                {"onClick", std::bind(&LoginComponent::onClick, this, _1)}
            }, {
                C(E::Text, {{"textContent", "Login"}})
            })
        });
}