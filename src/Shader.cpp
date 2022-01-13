#include "Shader.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <glad/glad.h>

Shader::Shader() : _programID(-1) { }

Shader::~Shader() {
    glDeleteProgram(_programID);
}

GLint Shader::programID() {
    return _programID;
}

void Shader::use() {
    glUseProgram(_programID);
}

GLuint Shader::loadShaders(const std::string& vertexFilePath, const std::string& fragmentFilePath) {
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertexFilePath, std::ios::in);
    if(VertexShaderStream.is_open()){
        std::string Line = "";
        while(getline(VertexShaderStream, Line)) {
            VertexShaderCode += "\n" + Line;
        }
        VertexShaderStream.close();
    }else{
        std::cout << "Can't open " <<  vertexFilePath << "\n";
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragmentFilePath, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line)) {
            FragmentShaderCode += "\n" + Line;
        }
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;


    // Compile Vertex Shader
    std::cout << "Compiling shader" <<  vertexFilePath << "\n";
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(vertexShaderID, 1, &VertexSourcePointer , nullptr);
    glCompileShader(vertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(vertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
        printf("%s", &VertexShaderErrorMessage[0]);
    }


    // Compile Fragment Shader
    std::cout << "Compiling shader" <<  vertexFilePath << "\n";
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(fragmentShaderID, 1, &FragmentSourcePointer , nullptr);
    glCompileShader(fragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(fragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
        printf("%s", &FragmentShaderErrorMessage[0]);
    }


    // Link the program

    printf("Linking program\n");
    _programID = glCreateProgram();
    glAttachShader(_programID, vertexShaderID);
    glAttachShader(_programID, fragmentShaderID);
    glLinkProgram(_programID);

    // Check the program
    glGetProgramiv(_programID, GL_LINK_STATUS, &Result);
    glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(_programID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
        printf("%s", &ProgramErrorMessage[0]);
    }

    glDetachShader(_programID, vertexShaderID);
    glDetachShader(_programID, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return _programID;
}

GLuint Shader::loadComputeShader(const std::string& computeShaderPath) {
    GLuint compShader = glCreateShader(GL_COMPUTE_SHADER);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    std::string ComputeShaderCode;
    std::ifstream shaderStream(computeShaderPath, std::ios::in);
    if(shaderStream.is_open()){
        std::string Line = "";
        while(getline(shaderStream, Line)) {
            ComputeShaderCode += "\n" + Line;
        }
        shaderStream.close();
    }else{
        std::cout << "Can't open " << computeShaderPath << std::endl;
        return 0;
    }

    const char* compShaderPtr = ComputeShaderCode.c_str();

    std::cout << "Compiling Compute shader: " << computeShaderPath << "\n";
    glShaderSource(compShader, 1, &compShaderPtr, nullptr);
    glCompileShader(compShader);

    // Check Compute Shader
    glGetShaderiv(compShader, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(compShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        printf("Error in compiling compute shader\n");
        std::vector<char> shaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(compShader, InfoLogLength, nullptr, &shaderErrorMessage[0]);
    }


    _programID = glCreateProgram();
    glAttachShader(_programID, compShader);
    glLinkProgram(_programID);

    glGetProgramiv(_programID, GL_LINK_STATUS, &Result);
    glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        printf("Error in linking compute shader\n");
        std::vector<char> shaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(compShader, InfoLogLength, nullptr, &shaderErrorMessage[0]);
        printf("%s", &shaderErrorMessage[0]);
    }

    return _programID;
}
