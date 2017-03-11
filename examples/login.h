// Copyright 2016 Dan Ristic

#include "opengl3_bridge.h"

#include <functional>

#include "src/context.h"

class LoginComponent {
 public:
    LoginComponent(std::string title)
        : mTitle{title}
        { }

    Nova::Element* render(Nova::Document& document);
    void HandleButtonClick(Nova::Event* event);
    void HandleInput(Nova::Event* event);

 private:
    std::string mTitle;
    std::string mUsername = "foobar";

    Nova::Element* mElement;
};

class Login {
 public:
    Login(Nova::Context& context, Nova::Div* root);

    LoginComponent loginComponent{"Cosmonaut"};
    std::function<void(void)> onlogin;
};
