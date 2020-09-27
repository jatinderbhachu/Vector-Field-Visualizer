#include <Window.h>

void error_callback(int error, const char* description)
{
    //printf("Error: %s\n", description);
}

void windowSizeCallback(GLFWwindow* window, int width, int height){
    //printf("Resize %i, %i\n", width, height);
    Camera::setViewPort(width, height);
    glViewport(0, 0, width, height);
}

void Window::cursorCallback(GLFWwindow* window, double xpos, double ypos){
    mouseMoving = true;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //printf("Scroll offsets: (%1.2f, %1.2f)\n", xoffset, yoffset);
    if(yoffset > 0){
        //Camera::zoom++;
        Camera::scale(0.95);
    } else {
        //Camera::zoom -= 0.5;
        Camera::scale(1.05);
    }
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    mKeyPresses[key].previous = mKeyPresses[key].current;
    mKeyPresses[key].current = action;
    KeyPress keypress = mKeyPresses[key];
    //if(!keypress.current && keypress.previous){
        //mKeyPresses[key].pressed = true;
        //mKeyPresses[key].previous = 0;
    //} else{
        //mKeyPresses[key].pressed = false;
    //}
    //printf("Key: %i, action: %i, previous: %i, current: %i\n", key, action,mKeyPresses[key].previous, mKeyPresses[key].current);
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    mMouseButtons[button].previous = mMouseButtons[button].current;
    mMouseButtons[button].current = action;
}

std::map<int, KeyPress> Window::mKeyPresses = {};
std::map<int, KeyPress> Window::mMouseButtons = {};

KeyPress Window::getKey(int key){
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

glm::vec2 Window::convertScreenToWorld(glm::vec2 pos){
    glm::vec3 converted = glm::unProject(glm::vec3(pos.x, pos.y, 0), glm::mat4(1.0), Camera::_projectionMatrix, glm::vec4(Camera::cameraPosition.x, Camera::cameraPosition.y, Camera::mWidth, Camera::mHeight));
    //converted += Camera::cameraPosition;
    return {converted.x + Camera::cameraPosition.x, converted.y - Camera::cameraPosition.y};
}

Window::Window(char* title, int width, int height){
    mTitle = title;
    mWidth = width;
    mHeight = height;
    if(!init())
        glfwTerminate();
}

Window::~Window(){
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

bool Window::mouseMoving = false;

bool Window::init(){
    if(!glfwInit()){
        printf("Failed to init GLFW");
        return false;
    }


    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    
    const GLFWvidmode* mode = glfwGetVideoMode(primary);
    mWindow = glfwCreateWindow( mWidth, mHeight, mTitle, NULL, NULL);

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

    // Ensure we can capture keys being pressed
    glfwSetInputMode(mWindow, GLFW_STICKY_KEYS, GL_TRUE);




    printf("Window::init() done\n");

    glfwSetWindowUserPointer(mWindow, this);
    //glfwSetErrorCallback(error_callback);
    glfwSetWindowSizeCallback(mWindow, windowSizeCallback);
    glfwSetCursorPosCallback(mWindow, cursorCallback);
    glfwSetScrollCallback(mWindow, scroll_callback);
    glfwSetKeyCallback(mWindow, key_callback);
    glfwSetMouseButtonCallback(mWindow, mouse_button_callback);

    //glEnable(GL_PROGRAM_POINT_SIZE); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // wireframe

    int textureUnits;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
    printf("Max textureUnits: %i\n", textureUnits);
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

    // Setup style
    ImGui::StyleColorsDark();

    return true;
}



void Window::clear(){

    // 37,41,52
    //glClearColor(30.0/255.0, 35.0/255.0, 45.0/255.0, 1.0f);
    glClearColor(0.1, 0.1, 0.1, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
}

void Window::update(){
    // check for errors
    //GLenum error = glGetError();
    //if (error != GL_NO_ERROR)
        //printf("[ERROR] %u", error);

    mouseMoving = false;

    //if(glfwGetKey(mWindow, GLFW_KEY_ESCAPE)) {
        //glfwSetWindowShouldClose(mWindow, 1);
    //}


    glfwSwapBuffers(mWindow);
}

void Window::toggleVsync(){
    if(mVsyncEnabled){
        glfwSwapInterval(0);
        mVsyncEnabled = false;
    } else {
        glfwSwapInterval(1);
        mVsyncEnabled = true;
    }
}

bool Window::closed(){
    return glfwWindowShouldClose(mWindow) == 1;
}



