#include <Game.h>
#include <cstdio>
#include <stdlib.h>

Game::Game(){
    mWindow = new Window("VectorFieldVisualizer", 1280, 720);
    mCamera = new Camera();

    ResourceManager::LoadShader("default", "vertshader.vs", "fragshader.fs");
    ResourceManager::LoadShader("particle", "particle.vs","particle.fs");
    ResourceManager::LoadTexture("nani", "nani.png");
    ResourceManager::LoadComputeShader("computeShader", "computeShader.cs");

    ResourceManager::GetShader("default")->use();

    glPointSize(2);


    vectorField = new GpuParticle(50000, 5,5, ResourceManager::GetShader("particle"));
    vectorField->reload(xInput, yInput, zInput);

    axis = new Wireframe(ResourceManager::GetShader("particle"));
    plane = new Wireframe(ResourceManager::GetShader("particle"));
    axis->generateAxis(100.0f);
    plane->generateSquare(glm::vec3(0, 0, 0), 100.0f);

    // delta time calculation
    lastTime = 0.0f;
}

Game::~Game(){
    ResourceManager::Clear();
}

void Game::run() {

  while (!mWindow->closed()) {

    this->handleInput();

    // calculate delta time
    delta = glfwGetTime() - lastTime;

    // update last time
    lastTime = glfwGetTime();

    timer += delta;

    mCamera->update();


    ResourceManager::GetShader("default")->use();

    /* RENDERING */
    mWindow->clear();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("OPTIONS");

    mouseWorldCoords = Window::convertScreenToWorld(mousePos);

    ImGui::Text("%.3f ms, %.1f FPS", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);
    //ImGui::Text("Time: %.3f", glfwGetTime());
    //ImGui::Text("Cursor location: (%1.2f, %1.2f)\n", mousePos.x, mousePos.y);
    ImGui::Text("Camera location: (%1.2f, %1.2f)\n", mCamera->cameraPosition.x, mCamera->cameraPosition.y);
    //ImGui::Text("Cursor World location: (%1.2f, %1.2f)\n", mouseWorldCoords.x, mouseWorldCoords.y);

    ImGui::InputText("X input", &xInput);
    ImGui::InputText("Y input", &yInput);
    ImGui::InputText("Z input", &zInput);
    if(ImGui::Button("APPLY")) {
        vectorField->reload(xInput, yInput, zInput);
    }
    ImGui::NewLine();

    if(ImGui::Button("Reset")) {
        vectorField->reset();
    }


    if (ImGui::Button("Start/Stop"))
        mSimulationRunning = !mSimulationRunning;

    if(ImGui::SliderInt("Point Size", &mPointSize, 1, 5)) {
        glPointSize(mPointSize);
    }

    if (ImGui::Button("Toggle VSYNC"))
        mWindow->toggleVsync();

    ImGui::End();


    if(mSimulationRunning)
        vectorField->update(delta);

    vectorField->render();
    plane->render();
    axis->render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    mWindow->update();
  }
}

void Game::handleInput(){

    GLFWwindow* window = mWindow->getWindow();

    glfwPollEvents();
    glfwGetCursorPos(window, &mousePos.x, &mousePos.y);

    // if imgui window wants to use mouse, dont let glfw window use any inputs
    if (ImGui::GetIO().WantCaptureKeyboard) return;

    //if (mWindow->isKeyDown(GLFW_KEY_UP)) {
        //mCamera->boom(mCamSpeed * delta);
    //}
    //if (mWindow->isKeyDown(GLFW_KEY_DOWN)) {
        //mCamera->boom(-mCamSpeed * delta);
    //}
    //if (mWindow->isKeyDown(GLFW_KEY_LEFT)) {
        //mCamera->track(-mCamSpeed * delta);
    //}
    //if (mWindow->isKeyDown(GLFW_KEY_RIGHT)) {
        //mCamera->track(mCamSpeed * delta);
    //}

    // if imgui window wants to use mouse, dont let glfw window use any inputs
    if (ImGui::GetIO().WantCaptureMouse) return;

    if(mWindow->isMouseDown(GLFW_MOUSE_BUTTON_LEFT)){
        if(!dragging)
            prevMousePos = mousePos;
        dragging = true;

    }

    if(mWindow->isMouseReleased(GLFW_MOUSE_BUTTON_LEFT)){
        dragging = false;
    }

    if(Window::isMouseMoving()){
        if(dragging){
            deltaMousePos = prevMousePos-mousePos;
            mCamera->moveCamera({deltaMousePos.x, -deltaMousePos.y, 0});
            prevMousePos = mousePos;
        }
    }
}


