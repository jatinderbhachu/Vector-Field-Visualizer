#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <map>
#include <Camera.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"

struct KeyPress {
    bool previous, current;
};

class Window
{


    public:
        Window(char* title, int width, int height);

        ~Window();

        bool init(); // init the window, create opengl context 

        void clear(); // clear screen
        void update(); // input and swap buffers
        void toggleVsync();

        bool closed();

        GLFWwindow* getWindow() const { return mWindow; };
        inline int width() const { return mWidth; };
        inline int height() const { return mHeight; };

        static glm::vec2 convertScreenToWorld(glm::vec2);

        static std::map<int, KeyPress> mKeyPresses, mMouseButtons;

        static KeyPress getKey(int key);
        static bool isKeyPressed(int key);
        static bool isKeyDown(int key);
        static bool isKeyReleased(int key);

        static bool isMousePressed(int button);
        static bool isMouseDown(int button);
        static bool isMouseReleased(int button);
        static bool isMouseMoving() { return mouseMoving; };

    private:
        GLFWwindow* mWindow;
        int mWidth, mHeight;
        static bool mouseMoving;
        char* mTitle;

        static void key_callback(GLFWwindow*, int, int, int, int);
        static void mouse_button_callback(GLFWwindow*, int, int, int);
        static void cursorCallback(GLFWwindow* window, double xpos, double ypos);

        bool mVsyncEnabled = true;

};
