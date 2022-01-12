#include "GpuParticle.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>

GpuParticle::GpuParticle(int numParticles, int width, int height) : NUM_PARTICLES(numParticles), mWidth(width), mHeight(height)
{
    loadComputeShaderTemplate();

    glGenBuffers(1, &mParticleBuffer);
    glCreateVertexArrays(1, &mEmptyVAO);
    generateVecField();
}


GpuParticle::~GpuParticle(){
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mParticleBuffer);
    glDeleteBuffers(1, &mParticleBuffer);
}


void GpuParticle::loadComputeShaderTemplate() {
    std::string ComputeShaderCode;
    std::ifstream VertexShaderStream("../../../../resources/shaders/computeShaderTemplate.cs", std::ios::in);
    if(VertexShaderStream.is_open()){
        std::string Line = "";
        while(getline(VertexShaderStream, Line)) {
            ComputeShaderCode += "\n" + Line;
        }
        VertexShaderStream.close();
    } else {
        printf("Can't open compute shader template.\n");
        return;
    }

    computeShaderTemplate = ComputeShaderCode;
}

void GpuParticle::reload(std::string x, std::string y, std::string z) {
    xExpression = x;
    yExpression = y;
    zExpression = z;
    reloadShader();
    generateVecField();
}

void GpuParticle::reloadShader() {

    GLuint compShader = glCreateShader(GL_COMPUTE_SHADER);

    char* formattedShader = new char[(
                computeShaderTemplate.size() 
                + xExpression.size()*2
                + yExpression.size()*2 
                + zExpression.size()*2) ];

    sprintf(formattedShader, 
            computeShaderTemplate.c_str(), 
            xExpression.c_str(), 
            yExpression.c_str(), 
            zExpression.c_str(),
            xExpression.c_str(), 
            yExpression.c_str(), 
            zExpression.c_str()
            );

    // error logging
    GLint Result = GL_FALSE;
    int InfoLogLength;

    printf("Compiling Compute shader\n");
    glShaderSource(compShader, 1, &formattedShader, nullptr);
    glCompileShader(compShader);

    // Check Compute Shader
    glGetShaderiv(compShader, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(compShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        printf("Error in compiling compute shader\n");
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(compShader, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
        printf("%s", &FragmentShaderErrorMessage[0]);
    }


    GLuint computeShader = glCreateProgram();
    glAttachShader(computeShader, compShader);
    glLinkProgram(computeShader);

    printf("Linking Compute Program\n");
    glGetProgramiv(computeShader, GL_LINK_STATUS, &Result);
    glGetProgramiv(computeShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        printf("Error in linking compute shader\n");
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(compShader, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
        printf("%s", &FragmentShaderErrorMessage[0]);
    }

    delete [] formattedShader;
    glad_glDeleteShader(compShader);

    glDeleteProgram(mComputeShader);

    mComputeShader = computeShader;

}

void GpuParticle::generateVecField() {

    std::vector<Particle> particleData;
    particleData.resize(NUM_PARTICLES);

    float cubeSize = std::cbrt(NUM_PARTICLES);
    float increment = mVecFieldSize.x/cubeSize;
    if(increment < 0.5) increment = 0.5;

    float x = -(mVecFieldSize.x)/2;
    float y = -(mVecFieldSize.y)/2;
    float z = -(mVecFieldSize.z)/2;

    for (int i = 0; i < NUM_PARTICLES; i++){
        float lifetime = 0.5f + (rand() / (double)RAND_MAX)*mLifetime;
        glm::vec4 spawn;
        spawn.x = x + mVecFieldPos.x;
        spawn.y = y + mVecFieldPos.y;
        spawn.z = z + mVecFieldPos.z;
        spawn.w = lifetime;

        particleData[i] = {spawn, spawn, glm::vec4(0.0f)};

        x += increment;
        if(x >= (mVecFieldSize.x)/2){
            y += increment;
            x = -(mVecFieldSize.x)/2;
        }
        if(y >= (mVecFieldSize.y)/2){
            y = -(mVecFieldSize.y)/2;
            z += increment;
        }
        if(z >= (mVecFieldSize.z)/2){
            z = -(mVecFieldSize.z)/2;
        }
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mParticleBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(Particle), particleData.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void GpuParticle::update(float delta) {
    glUseProgram(mComputeShader);
    glUniform1f(glGetUniformLocation(mComputeShader, "timestep"), delta);
    glUniform1f(glGetUniformLocation(mComputeShader, "speedMultiplier"), mSpeedMultiplier);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mParticleBuffer);
    glDispatchCompute((NUM_PARTICLES/128) + 1, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void GpuParticle::render(Camera* camera) {
    ResourceManager::GetShader("particle")->use();
    const GLint& particleShaderID = ResourceManager::GetShader("particle")->programID();
    glBindVertexArray(mEmptyVAO);
    glm::mat4 modelMatrix(1.0f);
    modelMatrix = glm::translate(modelMatrix, mVecFieldPos);

    glUniform2fv(glGetUniformLocation(particleShaderID, "particleScale"), 1, glm::value_ptr(mParticleScale));
    glUniformMatrix4fv(glGetUniformLocation(particleShaderID, "cameraView"),  1, GL_FALSE, glm::value_ptr(camera->viewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(particleShaderID, "cameraProjection"),  1, GL_FALSE, glm::value_ptr(camera->projectionMatrix()));

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mParticleBuffer);
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, NUM_PARTICLES);
}

