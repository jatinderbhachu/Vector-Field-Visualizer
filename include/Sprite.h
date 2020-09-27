#pragma once
#include <Renderable.h>
#include <Texture.h>


class Sprite : public Renderable{

    public:
        Sprite(glm::vec3 pos, glm::vec2 size, float rotation, glm::vec4 color, Texture tex);

        Texture getTexture() const { return mTexture; };
        float getRotation() const { return mRotation; };
        void setRotation(float rotation) { mRotation = rotation; };


    private:
        Texture mTexture;
        float mRotation;


};
