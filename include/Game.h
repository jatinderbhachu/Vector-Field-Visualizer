#pragma once

#include "glm/fwd.hpp"
#include <Window.h>
#include <BatchRenderer.h>
#include <SimpleRenderer.h>
#include <ResourceManager.h>
#include <Sprite.h>
#include <buffers/FrameBuffer.h>
#include <Texture.h>
#include <GpuParticle.h>


class Game
{
    public:
        Game();
        ~Game();

        void run();


    private:

        Window* mWindow;
        Camera* mCamera;

        GpuParticle* testParticle;

        void handleInput();
        double timer = 0.0f, delta = 0.0f, lastTime;

        glm::dvec2 mousePos = {0,0}, startMousePos, prevMousePos = {0,0}, deltaMousePos;
        glm::vec2 mouseWorldCoords = {0,0};
        bool dragging = false;

        int mPointSize = 1;
        bool mSimulationRunning = true;

        std::string testInput;

        std::string xInput = "-y", yInput = "y+x", zInput = "100";

};
