#pragma once

#include <string>
#include <glad/glad.h>

class Shader
{
    public:
        Shader();
        ~Shader();

        GLint programID();
        void use();


        GLuint loadShaders(const std::string& vertexFilePath, const std::string& fragmentFilePath);
        GLuint loadComputeShader(const std::string& computeShaderPath);

    private:
        GLint _programID;
};

