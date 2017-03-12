// Copyright 2016 Dan Ristic

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include "src/context.h"

#include "opengl3_bridge.h"
#include "login.h"

float gLastTime = 0;
float gLastPrintTime = 0;
int gFrames = 0;

int gWidth = 1280;
int gHeight = 720;

// float getDelta() {
//   Uint32 currentTime = SDL_GetTicks();
//   float deltaTime = static_cast<float>(currentTime - gLastTime);
//   gLastTime = currentTime;

//   // Measure speed
//   gFrames++;
//   if (currentTime - gLastPrintTime >= 1000.0) {
//     printf("%f ms/frame\n", 1000.0 / static_cast<double>(gFrames));
//     gFrames = 0;
//     gLastPrintTime += 1000.0;
//   }

//   // Cap frame rate
//   if (deltaTime < (1000.0 / 60.0) && deltaTime > 0) {
//       SDL_Delay((1000.0 / 60.0) - deltaTime);
//   }

//   return deltaTime;
// }

int main() {
    Nova::Context context;
    Nova::Document& document = context.document;

    printf("Starting...!\n");

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_DECORATED, GL_FALSE);
    window = glfwCreateWindow(gWidth, gHeight, "Cosmonaut", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    document.setDimensions(gWidth, gHeight);

    auto adaptor = std::make_shared<Nova::OpenGL3Bridge>();
    context.useAdaptor(adaptor);

    context.renderer.loadFont(context, "Roboto", "assets/Roboto-Regular.ttf");
    context.renderer.loadFont(context, "Roboto Thin", "assets/Roboto-Thin.ttf");

    Nova::Div* div = document.createElement<Nova::Div>();
    div->style.width = 1280;
    div->style.height = 720;
    div->style.backgroundImage = "assets/bg.png";

    auto loginComponent = std::make_shared<LoginComponent>(gWidth, gHeight);
    context.component.render(div, loginComponent);

    document.body->append(div);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int focused = glfwGetWindowAttrib(window, GLFW_FOCUSED);
        if (focused) {
            context.setMousePosition(xpos, ypos);
        } else {
            context.setMousePosition(-1, -1);
        }

        context.update(16);
        context.render(16);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    //     // SDL_Cursor* cursor;
    //     // cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    //     // SDL_SetCursor(cursor);

    //     // Update screen
    //     SDL_GL_SwapWindow(gWindow);

    //     while (SDL_PollEvent(&event) != 0) {
    //         switch (event.type) {
    //             case SDL_MOUSEWHEEL:
    //                 if (event.wheel.y > 0) {
    //                     context.setScrollDirection(1);
    //                 } else if (event.wheel.y < 0) {
    //                     context.setScrollDirection(-1);
    //                 }
    //                 break;
    //             case SDL_MOUSEBUTTONDOWN:
    //                 if (event.button.button == SDL_BUTTON_LEFT) {
    //                     context.setMouseDown();
    //                 }
    //                 break;
    //             case SDL_TEXTINPUT:
    //                 context.addInputCharacters(event.text.text);
    //                 break;
    //             case SDL_KEYDOWN:
    //                 switch (event.key.keysym.sym) {
    //                     case SDLK_BACKSPACE:
    //                         context.setKeyPress(Nova::KeyType::Backspace);
    //                         break;
    //                     default:
    //                         break;
    //                 }
    //                 break;
    //             case SDL_QUIT:
    //                 quit = true;
    //                 break;
    //         }
    //     }
    // } while (!quit);

    // videoThread.join();

    return 0;
}

