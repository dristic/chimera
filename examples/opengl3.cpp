// Copyright 2016 Dan Ristic

#include <SDL.h>

#include "src/context.h"

#include "opengl3_bridge.h"
#include "login.h"
#include "browse.h"

SDL_Window* gWindow;
SDL_Surface* gScrenSurface;
SDL_GLContext gContext;

float gLastTime = 0;
float gLastPrintTime = 0;
int gFrames = 0;

int gWidth = 1280;
int gHeight = 720;

void initSDL(int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

    // Create window
    gWindow = SDL_CreateWindow(
        "Materia",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);
    if (gWindow == NULL) {
        printf("Window could not be created %s\n", SDL_GetError());
    }

    // Get window surface
    gContext = SDL_GL_CreateContext(gWindow);
    SDL_GL_MakeCurrent(gWindow, gContext);

    if (gContext == NULL) {
        printf(
            "OpenGL context could not be created! SDL_Error: %s\n",
            SDL_GetError());
    }
}

float getDelta() {
  Uint32 currentTime = SDL_GetTicks();
  float deltaTime = static_cast<float>(currentTime - gLastTime);
  gLastTime = currentTime;

  // Measure speed
  gFrames++;
  if (currentTime - gLastPrintTime >= 1000.0) {
    printf("%f ms/frame\n", 1000.0 / static_cast<double>(gFrames));
    gFrames = 0;
    gLastPrintTime += 1000.0;
  }

  // Cap frame rate
  if (deltaTime < (1000.0 / 60.0) && deltaTime > 0) {
      SDL_Delay((1000.0 / 60.0) - deltaTime);
  }

  return deltaTime;
}

#undef main

int main() {
    Nova::Context context;
    Nova::Document& document = context.document;

    printf("Starting...!\n");

    initSDL(gWidth, gHeight);

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

    bool quit = false;
    SDL_Event event;
    do {
        float dt = getDelta();

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        if (SDL_GetWindowFlags(gWindow) & SDL_WINDOW_MOUSE_FOCUS) {
            context.setMousePosition(mouseX, mouseY);
        } else {
            context.setMousePosition(-1, -1);
        }

        context.update(dt);
        context.render(dt);

        // SDL_Cursor* cursor;
        // cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
        // SDL_SetCursor(cursor);

        // Update screen
        SDL_GL_SwapWindow(gWindow);

        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                case SDL_MOUSEWHEEL:
                    if (event.wheel.y > 0) {
                        context.setScrollDirection(1);
                    } else if (event.wheel.y < 0) {
                        context.setScrollDirection(-1);
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        context.setMouseDown();
                    }
                    break;
                case SDL_TEXTINPUT:
                    context.addInputCharacters(event.text.text);
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_BACKSPACE:
                            context.setKeyPress(Nova::KeyType::Backspace);
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }
    } while (!quit);

    // videoThread.join();

    return 0;
}

