#include "Window.h"
#include "Camera.h"

#include <iostream>

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>


std::unordered_map<int, Window::KeyPress> Window::mKeyPresses = {};
std::unordered_map<int, Window::KeyPress> Window::mMouseButtons = {};
bool    Window::mMouseMoving = false;
bool    Window::mResizeFlag = false;
double  Window::mMouseWheelDelta = 0.0f;
int     Window::mWidth = 1280;
int     Window::mHeight = 720;

void error_callback(int error, const char* description) {
    //printf("Error: %s\n", description);
}

void Window::windowSizeCallback(GLFWwindow* window, int width, int height){
    mResizeFlag = true;
    mWidth = width;
    mHeight = height;
}

void Window::cursorCallback(GLFWwindow* window, double xpos, double ypos){
    mMouseMoving = true;
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    mMouseWheelDelta = yoffset;
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    mKeyPresses[key].previous = mKeyPresses[key].current;
    mKeyPresses[key].current = action;
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    mMouseButtons[button].previous = mMouseButtons[button].current;
    mMouseButtons[button].current = action;
}

Window::KeyPress Window::getKey(int key){
    return mKeyPresses[key];
}

bool Window::isKeyPressed(int key){
    if(!mKeyPresses[key].current && mKeyPresses[key].previous){
        mKeyPresses[key].previous = false;
        return true;
    } else {
        return false;
    }
}

bool Window::isKeyDown(int key){
    if(mKeyPresses[key].current == 1) return true;
    else return false;
}

bool Window::isKeyReleased(int key){
    if(mKeyPresses[key].current == 0) return true;
    else return false;
}


bool Window::isMousePressed(int button){
    if(!mMouseButtons[button].current && mMouseButtons[button].previous){
        mMouseButtons[button].previous = false;
        return true;
    } else {
        return false;
    }
}

bool Window::isMouseDown(int button){
    if(mMouseButtons[button].current == 1) return true;
    else return false;
}

bool Window::isMouseReleased(int button){
    if(mMouseButtons[button].current == 0) return true;
    else return false;
}

Window::Window(const std::string& title, int width, int height) : mTitle(title) {
    if(!init())
        glfwTerminate();
}

Window::~Window(){
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

bool Window::init(){
    if(!glfwInit()){
        printf("Failed to init GLFW");
        return false;
    }


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    mWindow = glfwCreateWindow( mWidth, mHeight, mTitle.c_str(), NULL, NULL);

    if( mWindow == nullptr ) {
        printf("Failed to open GLFW window.\n" );
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(mWindow);

    glfwSwapInterval(1); // VSYNC 0 OFF 1 ON

    if (!gladLoadGL()) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return false;
    }

    printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

    glfwSetInputMode(mWindow, GLFW_STICKY_KEYS, GL_TRUE);

    printf("Window::init() done\n");

    glfwSetWindowUserPointer(mWindow, this);
    glfwSetErrorCallback(error_callback);
    glfwSetWindowSizeCallback(mWindow, windowSizeCallback);
    glfwSetCursorPosCallback(mWindow, cursorCallback);
    glfwSetScrollCallback(mWindow, scrollCallback);
    glfwSetKeyCallback(mWindow, keyCallback);
    glfwSetMouseButtonCallback(mWindow, mouseButtonCallback);

    glDepthMask(GL_FALSE);

    int textureUnits;

    int work_grp_cnt[3];
    int work_grp_size[3];
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);

    printf("max global (total) work group counts x:%i y:%i z:%i\n",
            work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);

    printf("max local (in one shader) work group sizes x:%i y:%i z:%i\n",
            work_grp_size[0], work_grp_size[1], work_grp_size[2]);

    // Setup ImGui binding
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.WantCaptureMouse = false;
    io.WantCaptureKeyboard = true;
    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImGui::StyleColorsDark();

    return true;
}



void Window::clear(){
    glClearColor(0.1, 0.1, 0.1, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
}

void Window::update(){
    mMouseMoving = false;
    mResizeFlag = false;
    mMouseWheelDelta = 0.0f;
    glfwSwapBuffers(mWindow);
}

void Window::toggleVsync(){
    mVsyncEnabled = !mVsyncEnabled;
    glfwSwapInterval((int)mVsyncEnabled);
}

bool Window::closed(){
    return glfwWindowShouldClose(mWindow) == 1;
}



