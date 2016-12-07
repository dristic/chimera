// Copyright 2016 Dan Ristic

#include "browse.h"

Browse::Browse(Nova::Context& context, Nova::Div* root, Nova::OpenGL3Bridge& bridge) {
    Nova::Document& document = context.document;

    document.styleManager.addRule([](Nova::StyleRule& rule) {
        rule.addClass("iconBg");
        rule.withWidth(75)
            .withHeight(780)
            .withBackgroundColor(Nova::Color::fromRGBA(15.0f, 40.0f, 67.0f, 1.0f))
            .withJustifyContent(Nova::Align::Center)
            .withFlexDirection(Nova::Direction::Column);
    });

    Nova::Div* iconBg = document.createElement<Nova::Div>();
    iconBg->className = "iconBg";
    root->append(iconBg);

    auto gearIcon = document.createElement<Nova::Img>();
    gearIcon->src("assets/gear.png");
    gearIcon->style
        .withMargin({30, 0, 0, 0})
        .withWidth(32)
        .withHeight(32);
    iconBg->append(gearIcon);

    auto mailIcon = document.createElement<Nova::Img>();
    mailIcon->src("assets/mail.png");
    mailIcon->style
        .withMargin({30, 0, 0, 0})
        .withWidth(32)
        .withHeight(28);
    iconBg->append(mailIcon);

    auto cabinetIcon = document.createElement<Nova::Img>();
    cabinetIcon->src("assets/cabinet.png");
    cabinetIcon->style
        .withMargin({30, 0, 0, 0})
        .withWidth(32)
        .withHeight(28);
    iconBg->append(cabinetIcon);

    Nova::Div* browseBg = document.createElement<Nova::Div>();
    browseBg->style
        .withPadding({50, 15, 15, 15})
        .withWidth(250)
        .withHeight(780)
        .withBackgroundColor(Nova::Color::fromRGBA(11.0f, 30.0f, 50.0f, 1.0f))
        .withJustifyContent(Nova::Align::Center)
        .withFlexDirection(Nova::Direction::Column);
    root->append(browseBg);

    auto img = document.createElement<Nova::Img>();
    img->src("assets/logo.png");
    img->style.margin.top = 15;
    img->style.margin.bottom = 15;
    browseBg->append(img);

    Nova::Div* identity = document.createElement<Nova::Div>();
    identity->style
        .withWidth(250)
        .withHeight(400)
        .withJustifyContent(Nova::Align::Center)
        .withFlexDirection(Nova::Direction::Column);
    browseBg->append(identity);

    Nova::Div* name = document.createElement<Nova::Div>();
    name->style
        .withWidth(170)
        .withHeight(40)
        .withColor({1.0f, 1.0f, 1.0f, 1.0f});
    name->textContent = "User Name";
    identity->append(name);

    Nova::Div* status = document.createElement<Nova::Div>();
    status->style
        .withWidth(170)
        .withHeight(40)
        .withColor({88.0f/255.0f, 116.0f/255.0f, 146.0f/255.0f, 1.0f})
        .withFontSize(14);
    status->textContent = "Online";
    identity->append(status);

    auto navigation = document.createElement<Nova::Div>();
    navigation->style
        .withMargin({30, 0, 0, 0})
        .withWidth(170)
        .withHeight(250)
        .withFlexDirection(Nova::Direction::Column);
    identity->append(navigation);

    std::vector<std::string> navItems = {
        "Home",
        "Movies",
        "Games",
        "Apps"
    };
    for (auto& navItem : navItems) {
        auto navItemEl = document.createElement<Nova::Div>();
        navItemEl->style
            .withWidth(170)
            .withHeight(40)
            .withColor(Nova::Color::fromRGBA(255, 255, 255, 1.0f));
        navItemEl->textContent = navItem;
        navigation->append(navItemEl);
    }

    auto content = document.createElement<Nova::Div>();
    content->style
        .withPadding({10, 10, 10, 10})
        .withWidth(400)
        .withHeight(720)
        .withFlexDirection(Nova::Direction::Column);
    root->append(content);

    Nova::Img* gifImg = document.createElement<Nova::Img>();
    gifImg->style.width = 50;
    gifImg->style.height = 50;
    gifImg->src("assets/loader.gif");
    content->append(gifImg);

    auto video = document.createElement<Nova::Img>();
    video->style
        .withWidth(640)
        .withHeight(360);
    video->src("assets/space.mp4");
    content->append(video);

    auto textArea = document.createElement<Nova::Div>();
    textArea->style
        .withPadding({10, 10, 10, 10})
        .withWidth(400)
        .withHeight(320);
    root->append(textArea);

    auto description = document.createElement<Nova::Div>();
    description->style
        .withWidth(400)
        .withHeight(1500)
        .withColor(Nova::Color::fromRGBA(255, 255, 255, 1));
    description->textContent = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus cursus mollis purus in ullamcorper. In condimentum lectus eget justo ultrices, at consectetur enim placerat. Aliquam facilisis mattis dui, a tristique ex pulvinar euismod. Vestibulum a eleifend libero, sed ultrices augue. Ut in malesuada orci, nec maximus velit. Fusce sodales erat ac sapien fringilla sodales. Duis dolor ante, sagittis non elit in, interdum tristique magna. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer varius, dolor in aliquam tempor, erat tortor ultricies tortor, id egestas neque nunc at magna. Sed nec mauris accumsan, vestibulum leo non, molestie felis. Nam dictum volutpat erat. Fusce malesuada finibus erat pretium molestie. In semper ut sem eget aliquet. Quisque dignissim efficitur dui, pulvinar accumsan justo faucibus in. Mauris mollis turpis quis nisl molestie, accumsan vulputate nunc dignissim.\n\nNam vitae nibh quis lacus ornare gravida. Nam eu porta eros. Praesent in velit et risus ultricies vehicula in vel dolor. In nisi mauris, egestas sed felis ut, gravida tincidunt purus. Phasellus justo nisi, auctor sit amet tempor ut, porttitor ultrices augue. In dignissim lorem est, non malesuada urna accumsan vel. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Integer enim elit, tristique non ex vitae, pharetra aliquet enim. Sed vitae felis nisl. Integer lacus ante, placerat ut mi quis, suscipit dapibus nisl. Vestibulum lacinia, orci nec aliquet bibendum, enim mi luctus magna, at rhoncus massa justo nec lorem. Aliquam et augue libero. Nullam volutpat, ligula vel consequat consequat, ante enim viverra lectus, pharetra accumsan lacus dui at odio. Suspendisse vitae massa tincidunt, fringilla nisl sit amet, condimentum orci. Curabitur at nulla consectetur, ornare sem quis, consequat libero. Praesent dictum dictum ante, vel rhoncus urna.";
    textArea->append(description);
}
