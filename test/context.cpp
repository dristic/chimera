// Copyright 2016 Dan Ristic

#include <iostream>
#include "gtest/gtest.h"

#include "src/rendering.h"
#include "src/context.h"

namespace ChimeraTest {

class ContextTest : public ::testing::Test {
 protected:
    ContextTest() : context{} { }

    virtual ~ContextTest() { }

    virtual void SetUp() { }

    virtual void TearDown() { }

    Chimera::Context context;
};

TEST_F(ContextTest, SetsMousePositionCorrectly) {
    int x = 10;
    int y = 50;

    context.setMousePosition(x, y);

    Chimera::Vec2 position = context.getMousePosition();

    EXPECT_EQ(x, position.x);
    EXPECT_EQ(y, position.y);
}

}  // namespace ChimeraTest
