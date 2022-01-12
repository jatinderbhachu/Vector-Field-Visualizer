#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>


class Window
{
struct KeyPress {
    bool previous, current;
};

public:
    Window(const std::string& title, int width, int height);

    ~Window();

    bool init();

    void clear();
    void update();
    void toggleVsync();

    bool closed();

    inline bool resized() { return mResizeFlag; }

    inline GLFWwindow* getWindow() const { return mWindow; }
    inline int width() const { return mWidth; }
    inline int height() const { return mHeight; }

    static KeyPress getKey(int key);
    static bool isKeyPressed(int key);
    static bool isKeyDown(int key);
    static bool isKeyReleased(int key);

    static bool isMousePressed(int button);
    static bool isMouseDown(int button);
    static bool isMouseReleased(int button);
    static bool isMouseMoving() { return mMouseMoving; }
    static double getMouseWheelDelta() { return mMouseWheelDelta; }

private:
    GLFWwindow* mWindow;
    std::string mTitle;
    static int mWidth, mHeight;
    static bool mMouseMoving;
    static bool mResizeFlag;

    static void keyCallback(GLFWwindow*, int, int, int, int);
    static void mouseButtonCallback(GLFWwindow*, int, int, int);
    static void cursorCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void windowSizeCallback(GLFWwindow* window, int width, int height);

    bool mVsyncEnabled = true;

    static std::unordered_map<int, KeyPress> mKeyPresses, mMouseButtons;
    static double mMouseWheelDelta;
};
