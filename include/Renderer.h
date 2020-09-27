#pragma once
// Include GLAD
#include <glad/glad.h>
#include <stdio.h>
#include <Renderable.h>

#include <Sprite.h>
#include <Camera.h>

class Renderer
{
    public:
        struct VertexData
        {
            glm::vec3 position;
            glm::vec4 colour; 
            glm::vec2 texCoord;
            float texID;
        };
        virtual void submit(const Renderable* renderable) = 0;
        virtual void flush() = 0;
};

