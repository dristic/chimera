// Copyright 2016 Dan Ristic

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include "src/context.h"

#include "opengl3_bridge.h"
#include "login.h"

int gWidth = 1280;
int gHeight = 720;

void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main() {
    Nova::Context context;
    Nova::Document& document = context.document;

    printf("Starting...!\n");

    glfwSetErrorCallback(errorCallback);

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
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

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    document.setScreenDimensions(width, height);

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

    double lastTime = glfwGetTime();
    int frames = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Measure speed
        double currentTime = glfwGetTime();
        frames++;
        if (currentTime - lastTime >= 1.0) {
            printf("%f ms/frame\n", 1000.0/double(frames));
            frames = 0;
            lastTime += 1.0;
        }

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int focused = glfwGetWindowAttrib(window, GLFW_FOCUSED);
        if (focused) {
            context.setMousePosition(xpos, ypos);
        } else {
            context.setMousePosition(-1, -1);
        }

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        document.setScreenDimensions(width, height);

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

