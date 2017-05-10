// Copyright 2016 Dan Ristic

#include <string>
#include <functional>

#include "Cosmonaut/Cosmonaut.h"

#include "src/adaptor/opengl3.h"
#include "app/login.h"
#include "app/loading.h"

#ifdef _WIN32
    #include <Windows.h>
#endif

using namespace std::placeholders;

// from: http://stackoverflow.com/questions/19968705/unsigned-integer-as-utf-8-value
std::string UnicodeToUTF8(unsigned int codepoint)
{
    std::string out;

    if (codepoint <= 0x7f)
        out.append(1, static_cast<char>(codepoint));
    else if (codepoint <= 0x7ff)
    {
        out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
        out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    else if (codepoint <= 0xffff)
    {
        out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
        out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
        out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    else
    {
        out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
        out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
        out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
        out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
    }
    return out;
}

class GLFWapplication {
 public:
    static void initialize(GLFWwindow* window, Nova::Context* context) {
        mContext = context;

        glfwSetErrorCallback(errorCallback);
        glfwSetScrollCallback(window, scrollCallback);
        glfwSetMouseButtonCallback(window, mouseButtonCallback);
        glfwSetCursorPosCallback(window, cursorPositionCallback);
        glfwSetCharCallback(window, characterCallback);
        glfwSetKeyCallback(window, keyCallback);
    }

    static void errorCallback(int error, const char* description) {
        fprintf(stderr, "Error: %s\n", description);
    }

    static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
        int direction = 0;

        if (yOffset > 0) {
            direction = 1;
        } else if (yOffset < 0) {
            direction = -1;
        }

        mContext->setScrollDirection(direction);
    }

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            mContext->setMouseDown();
        }
    }

    static void cursorPositionCallback(GLFWwindow* window, double x, double y) {
        int focused = glfwGetWindowAttrib(window, GLFW_FOCUSED);
        if (focused) {
            mContext->setMousePosition(x, y);
        } else {
            mContext->setMousePosition(-1, -1);
        }
    }

    static void characterCallback(GLFWwindow* window, unsigned int codepoint) {
        std::string text = UnicodeToUTF8(codepoint);

        mContext->addInputCharacters(text);
    }

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
            mContext->setKeyPress(Nova::KeyType::Backspace);
        }
    }

 private:
    GLFWapplication() { }
    GLFWapplication(GLFWapplication const&);
    void operator=(GLFWapplication const&);

    static Nova::Context* mContext;
};

Nova::Context* GLFWapplication::mContext{nullptr};

#ifdef _WIN32
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#else
int main() {
#endif
    const int WIDTH = 1280;
    const int HEIGHT = 720;

    Nova::Context context;
    Nova::Document& document = context.document;

    printf("Starting...!\n");

#ifdef _WIN32
    glewInit();
#endif

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
    window = glfwCreateWindow(WIDTH, HEIGHT, "Cosmonaut", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primary);

    int centerX = (mode->width - WIDTH) / 2;
    int centerY = (mode->height - HEIGHT) / 2;
    glfwSetWindowPos(window, centerX, centerY);

    GLFWapplication::initialize(window, &context);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    document.setDimensions(WIDTH, HEIGHT);

    auto adaptor = std::make_shared<Nova::OpenGL3Bridge>();
    context.useAdaptor(adaptor);

    context.renderer.loadFont(context, "Roboto", "assets/Roboto-Regular.ttf");
    context.renderer.loadFont(context, "Roboto Thin", "assets/Roboto-Thin.ttf");

    auto loadingComponent = std::make_shared<LoadingComponent>();
    context.component.render(document.body, loadingComponent);

    GLFWcursor* arrowCursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    GLFWcursor* handCursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    GLFWcursor* ibeamCursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);

    double lastTime = glfwGetTime();
    int frames = 0;

    glfwSwapInterval(1);

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

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        document.setScreenDimensions(width, height);

        context.update(16);
        context.render(16);

        Nova::CursorType cursor = document.getCursorType();
        switch(cursor) {
        case Nova::CursorType::Arrow:
        {
            glfwSetCursor(window, arrowCursor);
            break;
        }
        case Nova::CursorType::Hand:
        {
            glfwSetCursor(window, handCursor);
            break;
        }
        case Nova::CursorType::IBeam:
        {
            glfwSetCursor(window, ibeamCursor);
            break;
        }
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwDestroyCursor(ibeamCursor);

    glfwTerminate();

    return 0;
}

