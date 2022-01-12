#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>

class GpuParticle;
class Camera;
class Window;
class Wireframe;

class Game
{
public:
    Game();
    ~Game();

    void run();
private:
    std::unique_ptr<Window> mWindow;
    std::unique_ptr<Camera> mCamera;
    std::unique_ptr<GpuParticle> mVectorField;
    std::unique_ptr<Wireframe> mAxis;
    std::unique_ptr<Wireframe> mPlane;

    void handleInput();
    double mDelta = 0.0f; 
    double mLastTime = 0.0f;

    glm::vec2 mMousePos = {0,0};
    glm::vec2 mPrevMousePos = {0,0};
    glm::vec2 mStartMousePos = {0, 0};
    glm::vec2 mDeltaMousePos = {0, 0};
    glm::vec2 mMouseWorldCoords = {0,0};
    bool mDragging = false;

    bool mSimulationRunning = true;

    //std::string xInput = "-y", yInput = "y+x", zInput = "100";
    std::string xInput = "z*z", yInput = "x*x", zInput = "y*y";

};
