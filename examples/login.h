// Copyright 2016 Dan Ristic

#include "src/component/component.h"

class LoginComponent : public Cosmonaut::Component {
public:
    LoginComponent(int width, int height)
        : Cosmonaut::Component()
        , mWidth{width}
        , mHeight{height}
        { }

    void onClick(Nova::Event* event);
    std::shared_ptr<Cosmonaut::Api::CElement> render(Nova::Context& context) override;

 private:
    int mWidth;
    int mHeight;
};
