#include "Wireframe.h"
#include "ResourceManager.h"
#include "buffers/Buffer.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "Camera.h"
#include "Shader.h"

#include <iostream>


void Wireframe::generateBuffers(std::vector<float>& vertices, std::vector<uint32_t>& indices) {
    std::cout << vertices.size() << std::endl;

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVertexBuffer);
    glGenBuffers(1, &mIndexBuffer);

    glBindVertexArray(mVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);
}

void Wireframe::generateLine(glm::vec3 p1, glm::vec3 p2) {
    std::vector<float> vertices = {
        // positions      // colors
        p1.x, p1.y, p1.z, 1.0f, 1.0f, 1.0f, 
        p2.x, p2.y, p2.z, 1.0f, 1.0f, 1.0f
    };

    std::vector<uint32_t> indices = {0, 1};

    mNumIndices = indices.size();

    generateBuffers(vertices, indices);
}


void Wireframe::generateAxis(float size) {
    std::vector<float> vertices = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        size, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, size, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, size, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    std::vector<uint32_t> indices = {0, 1, 2, 3, 4, 5};

    mNumIndices = indices.size();

    generateBuffers(vertices, indices);
}

void Wireframe::generateSquare(glm::vec3 pos, float size) {
    std::vector<float> vertices = {
        size,  0.0f,   size, 1.0f, 1.0f, 1.0f,
        size,  0.0f,  -size, 1.0f, 1.0f, 1.0f,
        -size, 0.0f,  -size, 1.0f, 1.0f, 1.0f,
        -size, 0.0f,   size, 1.0f, 1.0f, 1.0f,
    };

    std::vector<uint32_t> indices = {0, 1, 1, 2, 2, 3, 3, 0};

    mNumIndices = indices.size();

    generateBuffers(vertices, indices);
}

void Wireframe::render(Camera* camera) {
    Shader* shader = ResourceManager::GetShader("default");
    shader->use();

    glm::mat4 MVP = camera->projectionMatrix() * camera->viewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shader->programID(), "MVP"),  1, GL_FALSE, glm::value_ptr(MVP));

    //glLineWidth(1.0f);
    glBindVertexArray(mVAO);
    glDrawElements(GL_LINES, mNumIndices, GL_UNSIGNED_INT, 0);
}
