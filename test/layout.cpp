// Copyright 2017 Dan Ristic

#include <iostream>
#include "gtest/gtest.h"

#include "src/style/style.h"
#include "src/layout.h"

namespace ChimeraTest {

class LayoutTest : public ::testing::Test {
 protected:
    LayoutTest()
        : style{}
        , layout{&style}
        { }

    virtual ~LayoutTest() { }

    virtual void SetUp() { }

    virtual void TearDown() { }

    Chimera::Layout layout;
    Chimera::Style style;
};

TEST_F(LayoutTest, OverridesWithIntrinsicDimensions) {
    layout.intrinsicWidth = 500;
    layout.intrinsicHeight = 300;

    layout.layout();

    EXPECT_EQ(500, layout.rect.width);
    EXPECT_EQ(300, layout.rect.height);
}

}  // namespace ChimeraTest
