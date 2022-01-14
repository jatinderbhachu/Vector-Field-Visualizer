#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"

class Camera;

class GpuParticle
{

    struct Particle {
        glm::vec4 position;
        glm::vec4 initialPosition;
        glm::vec4 velocity;
    };


public:
    GpuParticle(int numParticles, int width, int height);
    ~GpuParticle();

    void update(float delta);
    void render(Camera* camera);
    void reloadShader();
    void reload(std::string x, std::string y, std::string z);

    std::string computeShaderTemplate;
    std::string xExpression = "0";
    std::string yExpression = "0";
    std::string zExpression = "0";
    glm::vec3 mVecFieldSize = glm::vec3(4.0f);
    glm::vec3 mVecFieldPos = {0, 0, 0};
    glm::vec2 mParticleScale = {0.015f, 0.015f};
    float mSpeedMultiplier = 1.0f;
    float mLifetime = 10;
    uint32_t NUM_PARTICLES;

private:
    void loadComputeShaderTemplate();
    void generateVecField();

    uint32_t mNumActiveParticles;
    GLuint mParticleBuffer;
    GLuint mEmptyVAO = 0;
    GLuint mComputeShader = 0;
};
