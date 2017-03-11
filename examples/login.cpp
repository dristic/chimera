// Copyright 2016 Dan Ristic

#include "login.h"

class CDiv {
 public:
    CDiv() { }

    static CDiv Create() {
        return {};
    }

    CDiv& id(std::string id) {
        mId = id;
        return *this;
    }
    std::string id() {
        return mId;
    }

    CDiv& text(std::string text) {
        mText = text;
        return *this;
    }
    std::string text() {
        return mText;
    }

 private:
    std::string mId = "";
    std::string mText = "";
};

Nova::Element* reconcile(CDiv& root, Nova::Element* current, Nova::Document& document) {
    if (current) {
        if (current->id() != root.id()) {
            current->id(root.id());
        }
        if (current->text() != root.text()) {
            current->text(root.text());
        }
    } else {
        current = document.createElement<Nova::Div>()->id(root.id())->text(root.text());
    }
    
    return current;
}

Nova::Element* LoginComponent::render(Nova::Document& document) {
    using namespace Nova;
    using namespace std::placeholders;

    document.styleManager.addRule([](StyleRule& rule) {
        rule.addId("login-container");
        rule.withWidth(1280)
            .withHeight(720)
            .withJustifyContent(Align::Center)
            .withColor(Color::fromRGBA(255, 255, 255, 1))
            .withPadding({150, 0, 0, 0});
    });

    document.styleManager.addRule([](StyleRule& rule) {
        rule.addId("login-bg");
        rule.withWidth(320)
            .withHeight(420)
            .withBackgroundColor(Color::fromRGBA(255, 255, 255, 0.2f))
            .withJustifyContent(Align::Center)
            .withFlexDirection(Direction::Column);
    });

    document.styleManager.addRule([](StyleRule& rule) {
        rule.addId("title");
        rule.withWidth(170)
            .withHeight(40)
            .withColor(Color::fromRGBA(255, 255, 255, 1));
    });

    document.styleManager.addRule([](StyleRule& rule) {
        rule.addId("login-button");
        rule.withBackgroundColor(Color::fromRGBA(11, 37, 63, 1.0f))
            .withWidth(170)
            .withHeight(40)
            .withPadding({5, 0, 0, 0})
            .withColor({1.0f, 1.0f, 1.0f, 1.0f})
            .withTextAlign(Nova::Align::Center);
    });

    document.styleManager.addRule([](StyleRule& rule) {
        rule.addId("username");
        rule.withBackgroundColor(Color::fromRGBA(11, 37, 63, 1.0f))
            .withWidth(170)
            .withHeight(40)
            .withMargin({5, 0, 0, 15})
            .withColor({1.0f, 1.0f, 1.0f, 1.0f});
    });

    mElement =
        document.createElement<Div>()->id("login-container")->children({
            document.createElement<Div>()->id("login-bg")->children({
                document.createElement<Div>()->id("title")->text(mTitle),
                document.createElement<Button>()->id("login-button")->text("Login")
                    ->onEvent(EventType::MouseDown,
                        std::bind(&LoginComponent::HandleButtonClick, this, _1)),
                document.createElement<Input>()->id("username")
                    ->onEvent(EventType::Change,
                        std::bind(&LoginComponent::HandleInput, this, _1))
            })
        });

    // mElement = reconcile(
    //     CDiv::Create().id("login-container").text(mTitle),
    //     mElement,
    //     document
    // );

    // mTitle = "Cosmonaut 2";

    // mElement = reconcile(
    //     CDiv::Create().id("login-container").text(mTitle),
    //     mElement,
    //     document
    // );

    return mElement;
}

void LoginComponent::HandleButtonClick(Nova::Event* event) {
    mTitle = "Clicked!";
}

void LoginComponent::HandleInput(Nova::Event* event) {
    mUsername = dynamic_cast<Nova::TextInputEvent*>(event)->value;

    if (mElement) {
        Nova::Element* el = mElement->getElementById("login-button");

        if (el) {
            el->text(mUsername);
        }
    }
}

Login::Login(
    Nova::Context& context,
    Nova::Div* root
) {
    root->append(loginComponent.render(context.document));

    // auto video = document.createElement<Nova::Img>();
    // video->style
    //     .withWidth(960)
    //     .withHeight(720)
    //     .withPosition(Nova::Position::Absolute);
    // video->src("assets/space.mp4");
    // root->append(video);

    // // Exit Button
    // auto windowIcons = document.createElement<Nova::Div>();
    // windowIcons->style
    //     .withWidth(30)
    //     .withHeight(30)
    //     .withPadding({10, 10, 10, 10})
    //     .withPosition(Nova::Position::Absolute)
    //     .withLeft(1230);
    // root->append(windowIcons);

    // auto exitButton = document.createElement<Nova::Img>();
    // exitButton->style
    //     .withWidth(30)
    //     .withHeight(30);
    // exitButton->src("assets/exit_icon.png");
    // windowIcons->append(exitButton);

    // // Login container
    // auto loginContainer = document.createElement<Nova::Div>();
    // loginContainer->style
    //     .withWidth(1280)
    //     .withHeight(720)
    //     .withJustifyContent(Nova::Align::Right)
    //     .withPadding({150, 0, 0, 0});
    // root->append(loginContainer);

    // auto loginBg = document.createElement<Nova::Div>();
    // loginBg->style
    //     .withPadding({50, 15, 15, 15})
    //     .withWidth(320)
    //     .withHeight(420)
    //     .withJustifyContent(Nova::Align::Center)
    //     .withFlexDirection(Nova::Direction::Column)
    //     .withBackgroundColor(Nova::Color::fromRGBA(255, 255, 255, 0.2f));
    // loginContainer->append(loginBg);

    // context.animationController.addAnimation(loginBg,
    //     [](float curTime, Nova::Element* element) {
    //         float pos = -50 * curTime * (curTime-2);
    //         element->style.margin.top = 50 - pos;
    //         element->style.opacity = -1 * curTime * (curTime - 2);
    //     });

    // auto titleDiv = document.createElement<Nova::Div>();
    // titleDiv->style
    //     .withWidth(170)
    //     .withHeight(40)
    //     .withFontSize(32);
    // titleDiv->style.color = {1.0f, 1.0f, 1.0f, 1.0f};
    // titleDiv->textContent = "Login";
    // loginBg->append(titleDiv);

    // auto input = document.createElement<Nova::Input>();
    // input->style.width = 170;
    // input->style.height = 40;
    // input->style.color = {1.0f, 1.0f, 1.0f, 1.0f};
    // input->style.backgroundColor =
    //     {11.0f/255.0f, 37.0f/255.0f, 63.0f/255.0f, 1.0f};
    // input->style.borderWidth = 2;
    // input->style.borderColor =
    //     Nova::Color::fromRGBA(255, 255, 255, 1.0f);
    // input->style.margin.top = 5;
    // input->style.margin.bottom = 15;
    // loginBg->append(input);

    // auto password = document.createElement<Nova::Input>();
    // password->style.width = 170;
    // password->style.height = 40;
    // password->style.color = {1.0f, 1.0f, 1.0f, 1.0f};
    // password->style.backgroundColor =
    //     {11.0f/255.0f, 37.0f/255.0f, 63.0f/255.0f, 1.0f};
    // password->style.borderWidth = 2;
    // password->style.borderColor =
    //     Nova::Color::fromRGBA(255, 255, 255, 1.0f);
    // password->style.margin.top = 5;
    // password->style.margin.bottom = 15;
    // loginBg->append(password);

    // auto registerLink = document.createElement<Nova::Div>();
    // registerLink->style.width = 170;
    // registerLink->style.height = 40;
    // registerLink->style.textDecoration = Nova::TextDecoration::Underline;
    // registerLink->style.color =
    //     Nova::Color::fromRGBA(17, 56, 94, 1.0f);
    // registerLink->style.fontSize = 14;
    // registerLink->textContent = "Register";
    // loginBg->append(registerLink);

    // Nova::StyleRule buttonStyle{};
    // buttonStyle.addTag("button");
    // buttonStyle
    //     .withBackgroundColor(Nova::Color::fromRGBA(11, 37, 63, 1.0f))
    //     .withPadding({5, 0, 0, 0})
    //     .withWidth(170)
    //     .withHeight(40)
    //     .withColor({1.0f, 1.0f, 1.0f, 1.0f})
    //     .withTextAlign(Nova::Align::Center);
    // document.styleManager.addRule(std::move(buttonStyle));

    // Nova::StyleRule buttonHover{};
    // buttonHover.addTag("button");
    // buttonHover.addPseudoClass(Nova::PseudoClass::Hover);
    // buttonHover.withBackgroundColor(Nova::Color::fromRGBA(17, 56, 94, 1.0f));
    // document.styleManager.addRule(std::move(buttonHover));

    // auto button = document.createElement<Nova::Button>();
    // button->textContent = "Login";
    // button->on(Nova::EventType::MouseDown, [this](Nova::Event* event) {
    //     this->onlogin();
    // });
    // loginBg->append(button);
}
