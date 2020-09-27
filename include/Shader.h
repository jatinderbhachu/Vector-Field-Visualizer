#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <string.h>
using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader
{
    public:
        Shader();
        virtual ~Shader();

        GLint programID() { return _programID; };
        void use() { glUseProgram(_programID); };


        GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path);
        GLuint loadComputeShader(const char* computeShaderPath);

    private:
        GLint _programID;
};

