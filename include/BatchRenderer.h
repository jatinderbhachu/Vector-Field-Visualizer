#pragma once

#include "AnimatedSprite.h"
#include <Renderer.h>
#include <Shader.h>
#include <buffers/IndexBuffer.h>
#include <buffers/VertexArray.h>
#include <glm/gtx/vector_angle.hpp>
static const size_t RendererMaxQuads = 20000;
static const size_t RendererVertexCount = RendererMaxQuads * 4;
static const size_t RendererIndicesCount = RendererMaxQuads * 6;
static const size_t RendererMaxTextures = 32; // TODO: query driver for this value

class BatchRenderer : public Renderer
{
    public:
        BatchRenderer();
        ~BatchRenderer();
        void begin();
        void submit(const Renderable* renderable) override;
        void submit(const Sprite* sprite);
        void submit(const AnimatedSprite* animSprite);
        void setDefaultShader(Shader* shader) { mShader = shader; };
        void end();
        void flush() override;

        void checkMaxVerts();

    private:
        glm::mat4 _projectionMatrix;
        glm::mat4 _viewMatrix;
        glm::vec4 QuadVertexPositions[4];

        GLuint m_VAO;
        GLuint m_VBO;
        GLuint m_WhiteTexture;
        Shader* mShader;
        IndexBuffer* m_IBO;
        GLsizei m_IndexCount;

        VertexData* m_BufferBase = nullptr;
        VertexData* m_BufferPtr = nullptr;
        void init();
};
