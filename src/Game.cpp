#include "Game.h"
#include "ResourceManager.h"
#include "Window.h"
#include "Camera.h"
#include "GpuParticle.h"
#include "Wireframe.h"

#include <iostream>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <misc/cpp/imgui_stdlib.h>

Game::Game(){
    mWindow = std::make_unique<Window>("VectorFieldVisualizer", 1280, 720);
    mCamera = std::make_unique<Camera>();
    mCamera->setViewPort(1280, 720);

    ResourceManager::LoadShader("default", "vertshader.vs", "fragshader.fs");
    ResourceManager::LoadShader("particle", "particle.vs","particle.fs");
    ResourceManager::LoadComputeShader("computeShader", "computeShader.cs");

    ResourceManager::GetShader("default")->use();

    mVectorField = std::make_unique<GpuParticle>(50000, 5, 5);
    mVectorField->reload(xInput, yInput, zInput);

    mAxis = std::make_unique<Wireframe>(ResourceManager::GetShader("particle"));
    mPlane = std::make_unique<Wireframe>(ResourceManager::GetShader("particle"));
    mAxis->generateAxis(100.0f);
    mPlane->generateSquare(glm::vec3(0, 0, 0), 100.0f);

    // for delta time calculation
    mLastTime = 0.0f;
}

Game::~Game(){
    ResourceManager::Clear();
}

void Game::run() {

    while (!mWindow->closed()) {
        glfwPollEvents();
        handleInput();

        mDelta = glfwGetTime() - mLastTime;
        mLastTime = glfwGetTime();

        if(mWindow->resized()) {
            mCamera->setViewPort(mWindow->width(), mWindow->height());
            glViewport(0, 0, mWindow->width(), mWindow->height());
        }

        ResourceManager::GetShader("default")->use();
        mCamera->update();
        mWindow->clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("OPTIONS");

        ImGui::Text("%.3f ms, %.1f FPS", 1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
        ImGui::Text("Camera location: (%1.2f, %1.2f)\n", mCamera->mCameraPosition.x, mCamera->mCameraPosition.y);

        ImGui::InputScalar("Particle Count", ImGuiDataType_U32, &mVectorField->NUM_PARTICLES);
        ImGui::InputFloat("Particle Lifetime", &mVectorField->mLifetime);

        ImGui::Text("Spawn Container Size");
        ImGui::SliderFloat("Width", &mVectorField->mVecFieldSize.x, 1, 50);
        ImGui::SliderFloat("Height", &mVectorField->mVecFieldSize.y, 1, 50);
        ImGui::SliderFloat("Depth", &mVectorField->mVecFieldSize.z, 1, 50);

        ImGui::Text("Spawn Container Position");
        ImGui::DragFloat("X", &mVectorField->mVecFieldPos.x, 1.0f, -400, 400);
        ImGui::DragFloat("Y", &mVectorField->mVecFieldPos.y, 1.0f, -400, 400);
        ImGui::DragFloat("Z", &mVectorField->mVecFieldPos.z, 1.0f, -400, 400);

        ImGui::Text("Vector Field Equation");
        ImGui::InputText("X input", &xInput);
        ImGui::InputText("Y input", &yInput);
        ImGui::InputText("Z input", &zInput);
        if(ImGui::Button("APPLY")) {
            mVectorField->reload(xInput, yInput, zInput);
        }
        ImGui::NewLine();

        if (ImGui::Button("Start/Stop"))
            mSimulationRunning = !mSimulationRunning;

        static float particleX = 1.0f;
        static float particleY = 1.0f;
        ImGui::SliderFloat("Particle X Scale", &mVectorField->mParticleScale.x, 0.001f, 1.0f);
        ImGui::SliderFloat("Particle Y Scale", &mVectorField->mParticleScale.y, 0.001f, 1.0f);
        ImGui::SliderFloat("Speed Multiplier", &mVectorField->mSpeedMultiplier, 0.001f, 5.0f);

        if (ImGui::Button("Toggle VSYNC"))
            mWindow->toggleVsync();

        ImGui::End();

        if(mSimulationRunning)
            mVectorField->update(mDelta);

        mVectorField->render(mCamera.get());

        mPlane->render(mCamera.get());
        mAxis->render(mCamera.get());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        mWindow->update();
    }
}

void Game::handleInput(){
    GLFWwindow* window = mWindow->getWindow();

    double mouseWheelDelta = mWindow->getMouseWheelDelta();

    if(mouseWheelDelta > 0.0) {
        mCamera->mCameraPosition *= 0.95f;
    } else if(mouseWheelDelta < 0.0) {
        mCamera->mCameraPosition *= 1.05f;
    }

    double mouseX = 0.0f;
    double mouseY = 0.0f;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    mMousePos = {mouseX, mouseY};

    // if imgui window wants to use input, dont let glfw window use any inputs
    if (ImGui::GetIO().WantCaptureKeyboard || ImGui::GetIO().WantCaptureMouse) return;

    if(mWindow->isMouseDown(GLFW_MOUSE_BUTTON_LEFT)){
        if(!mDragging)
            mPrevMousePos = mMousePos;
        mDragging = true;
    }

    if(mWindow->isMouseReleased(GLFW_MOUSE_BUTTON_LEFT)){
        mDragging = false;
    }

    if(Window::isMouseMoving()){
        if(mDragging){
            mDeltaMousePos = mPrevMousePos-mMousePos;
            mCamera->rotateCamera({mDeltaMousePos.x, -mDeltaMousePos.y});

            mPrevMousePos = mMousePos;
        }
    }
}


