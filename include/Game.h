#pragma once

#include "StaticSprite.h"
#include "glm/fwd.hpp"
#include <Window.h>
#include <BatchRenderer.h>
#include <SimpleRenderer.h>
#include <ResourceManager.h>
#include <Sprite.h>
#include <buffers/FrameBuffer.h>
#include <Texture.h>
#include <GpuParticle.h>
#include <Wireframe.h>


class Game
{
    public:
        Game();
        ~Game();

        void run();


    private:

        Window* mWindow;
        Camera* mCamera;

        GpuParticle* vectorField;

        Wireframe* axis;
        Wireframe* plane;

        void handleInput();
        double timer = 0.0f, delta = 0.0f, lastTime;
        float particleLifetime = 10.0f;

        glm::dvec2 mousePos = {0,0}, startMousePos, prevMousePos = {0,0}, deltaMousePos;
        glm::vec2 mouseWorldCoords = {0,0};
        bool dragging = false;

        int numParticles = 100000;

        int mPointSize = 1;
        bool mSimulationRunning = true;


        glm::vec3 vecFieldSize = glm::vec3(5, 5, 5);
        glm::vec3 vecFieldPos = glm::vec3(0, 0, 0);


        std::string testInput;

        std::string xInput = "-y", yInput = "y+x", zInput = "100";

};
