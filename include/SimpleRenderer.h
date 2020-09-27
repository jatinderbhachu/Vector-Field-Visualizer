#pragma once
#include <Shader.h>
#include <Camera.h>
#include <Renderer.h>
#include <StaticSprite.h>
#include <AnimatedSprite.h>
#include <deque>


class SimpleRenderer : public Renderer {

    public:
        SimpleRenderer();
        ~SimpleRenderer();

        void submit(const Renderable* sprite) override;
        void flush() override;
        void renderAnimSprite(const Camera* camera, const AnimatedSprite* sprite);

    private:
        std::deque<const StaticSprite*> mRenderQueue;
        glm::mat4 _projectionMatrix;
        glm::mat4 _viewMatrix;


};
