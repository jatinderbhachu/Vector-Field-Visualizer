#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Renderable{

    protected:
        glm::vec3 mPosition;
        glm::vec2 mSize;
        glm::vec4 mColor;


    public:
        Renderable(glm::vec3 pos, glm::vec2 size, glm::vec4 color) :
            mPosition(pos), mSize(size), mColor(color) {  }

        virtual ~Renderable() {};

        inline const glm::vec3& getPosition() const { return mPosition; }
        inline const glm::vec2& getSize() const { return mSize; }
        inline const glm::vec4& getColor() const { return mColor; }


};
