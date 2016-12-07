// Copyright 2016 Dan Ristic

#include "opengl3_bridge.h"

#include <functional>

#include "src/context.h"

class Login {
 public:
    Login(Nova::Context& context, Nova::Div* root, Nova::OpenGL3Bridge& bridge);

    std::function<void(void)> onlogin;
};
