// Copyright 2016 Dan Ristic

#include "login.h"

Login::Login(
    Nova::Context& context,
    Nova::Div* root,
    Nova::OpenGL3Bridge& bridge
) {
    Nova::Document& document = context.document;

    auto video = document.createElement<Nova::Img>();
    video->style
        .withWidth(1280)
        .withHeight(720)
        .withPosition(Nova::Position::Absolute);
    //video->src("assets/space.mp4");
    root->append(video);

    // Exit Button
    auto windowIcons = document.createElement<Nova::Div>();
    windowIcons->style
        .withWidth(30)
        .withHeight(30)
        .withPadding({10, 10, 10, 10})
        .withPosition(Nova::Position::Absolute)
        .withLeft(1230);
    root->append(windowIcons);

    auto exitButton = document.createElement<Nova::Img>();
    exitButton->style
        .withWidth(30)
        .withHeight(30);
    exitButton->src("assets/exit_icon.png");
    windowIcons->append(exitButton);

    // Login container
    auto loginContainer = document.createElement<Nova::Div>();
    loginContainer->style
        .withWidth(1280)
        .withHeight(720)
        .withJustifyContent(Nova::Align::Center)
        .withPadding({150, 0, 0, 0});
    root->append(loginContainer);

    auto loginBg = document.createElement<Nova::Div>();
    loginBg->style
        .withPadding({50, 15, 15, 15})
        .withWidth(320)
        .withHeight(420)
        .withJustifyContent(Nova::Align::Center)
        .withFlexDirection(Nova::Direction::Column)
        .withBackgroundColor(Nova::Color::fromRGBA(255, 255, 255, 0.2f));
    loginContainer->append(loginBg);

    context.animationController.addAnimation(loginBg,
        [](float curTime, Nova::Element* element) {
            float pos = -50 * curTime * (curTime-2);
            element->style.margin.top = 50 - pos;
            element->style.opacity = -1 * curTime * (curTime - 2);
        });

    auto titleDiv = document.createElement<Nova::Div>();
    titleDiv->style
        .withWidth(170)
        .withHeight(40)
        .withFontSize(32);
    titleDiv->style.color = {1.0f, 1.0f, 1.0f, 1.0f};
    titleDiv->textContent = "Nova UI";
    loginBg->append(titleDiv);

    auto input = document.createElement<Nova::Input>();
    input->style.width = 170;
    input->style.height = 40;
    input->style.color = {1.0f, 1.0f, 1.0f, 1.0f};
    input->style.backgroundColor =
        {11.0f/255.0f, 37.0f/255.0f, 63.0f/255.0f, 1.0f};
    input->style.borderWidth = 2;
    input->style.borderColor =
        Nova::Color::fromRGBA(255, 255, 255, 1.0f);
    input->style.margin.top = 5;
    input->style.margin.bottom = 15;
    loginBg->append(input);

    auto password = document.createElement<Nova::Input>();
    password->style.width = 170;
    password->style.height = 40;
    password->style.color = {1.0f, 1.0f, 1.0f, 1.0f};
    password->style.backgroundColor =
        {11.0f/255.0f, 37.0f/255.0f, 63.0f/255.0f, 1.0f};
    password->style.borderWidth = 2;
    password->style.borderColor =
        Nova::Color::fromRGBA(255, 255, 255, 1.0f);
    password->style.margin.top = 5;
    password->style.margin.bottom = 15;
    loginBg->append(password);

    auto registerLink = document.createElement<Nova::Div>();
    registerLink->style.width = 170;
    registerLink->style.height = 40;
    registerLink->style.textDecoration = Nova::TextDecoration::Underline;
    registerLink->style.color =
        Nova::Color::fromRGBA(17, 56, 94, 1.0f);
    registerLink->style.fontSize = 14;
    registerLink->textContent = "Register";
    loginBg->append(registerLink);

    Nova::StyleRule buttonStyle{};
    buttonStyle.addTag("button");
    buttonStyle
        .withBackgroundColor(Nova::Color::fromRGBA(11, 37, 63, 1.0f))
        .withPadding({5, 0, 0, 0})
        .withWidth(170)
        .withHeight(40)
        .withColor({1.0f, 1.0f, 1.0f, 1.0f})
        .withTextAlign(Nova::Align::Center);
    document.styleManager.addRule(std::move(buttonStyle));

    Nova::StyleRule buttonHover{};
    buttonHover.addTag("button");
    buttonHover.addPseudoClass(Nova::PseudoClass::Hover);
    buttonHover.withBackgroundColor(Nova::Color::fromRGBA(17, 56, 94, 1.0f));
    document.styleManager.addRule(std::move(buttonHover));

    auto button = document.createElement<Nova::Button>();
    button->textContent = "Login";
    button->on(Nova::EventType::MouseDown, [this](Nova::Event* event) {
        this->onlogin();
    });
    loginBg->append(button);
}
