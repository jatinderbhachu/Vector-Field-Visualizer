#pragma once
#include <Texture.h>
#include <Shader.h>
#include <Renderable.h>
#include <buffers/Buffer.h>
#include <buffers/IndexBuffer.h>
#include <buffers/VertexArray.h>


class StaticSprite : public Renderable {
    public:
        StaticSprite(glm::vec3 pos, glm::vec2 size, Texture tex, Shader* shader);
        ~StaticSprite();

        void setTexture(Texture& texture);

        glm::vec2 getPos() const { return mPos; };
        float getRotation() const { return mRotation; };
        /* void bind() const { mVAO.bind(); mIBO.bind(); mTexture.bind(); }; */
        /* void unbind() const { mVAO.unbind(); mIBO.unbind(); }; */

        VertexArray* getVAO() const { return mVAO; };
        IndexBuffer* getIBO() const { return mIBO; };

        void setPos(glm::vec2 pos) { mPos = glm::vec3(pos.x, pos.y, 0.0f); };
        void setSize(glm::vec2 size) { mSize = size; };
        void setRotation(float rotation) { mRotation = rotation; };

        inline const glm::vec3& getPosition() const { /*printf("(%f, %f)\n", mPos.x, mPos.y);*/ return mPos; };
        inline const glm::vec2& getSize() const { return mSize; };

        Texture getTexture() const { return mTexture; };
        Shader* getShader() const { return mShader; };

    private:
        Texture mTexture;
        VertexArray* mVAO;
        IndexBuffer* mIBO;

        glm::vec3 mPos;
        float mRotation = 0.0f;
        glm::vec2 mSize;
        Shader* mShader;

};

