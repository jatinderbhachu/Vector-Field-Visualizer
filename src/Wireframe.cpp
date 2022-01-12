#include "Wireframe.h"
#include "buffers/Buffer.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "Camera.h"
#include "Shader.h"


Wireframe::Wireframe(Shader* shader) : mShader(shader) {  }

void Wireframe::generateLine(glm::vec3 p1, glm::vec3 p2) {

    GLfloat vertices[] = {
        // positions 
        p1.x, p1.y, p1.z,
        p2.x, p2.y, p2.z
    };

    numPoints = 2;

    glGenBuffers(1, &mLinePoints);
    glBindBuffer(GL_ARRAY_BUFFER, mLinePoints);
    glBufferData(GL_ARRAY_BUFFER, numPoints*3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    GLfloat colours[] = {
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f
    };

    glGenBuffers(1, &mLineColour);
    glBindBuffer(GL_ARRAY_BUFFER, mLineColour);
    glBufferData(GL_ARRAY_BUFFER, numPoints*3 * sizeof(GLfloat), colours, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Wireframe::generateAxis(float size) {
    GLfloat vertices[] = {
        size, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f, // X axis
        0.0f, size, 0.0f,   0.0f, 0.0f, 0.0f, // Y axis
        0.0f, 0.0f, size,   0.0f, 0.0f, 0.0f  // Z axis
    };

    numPoints = 6;


    glGenBuffers(1, &mLinePoints);
    glBindBuffer(GL_ARRAY_BUFFER, mLinePoints);
    glBufferData(GL_ARRAY_BUFFER, numPoints*3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    GLfloat colours[] = {
        1.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f, 
        0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,  
        0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f 
    };


    glGenBuffers(1, &mLineColour);
    glBindBuffer(GL_ARRAY_BUFFER, mLineColour);
    glBufferData(GL_ARRAY_BUFFER, numPoints*3 * sizeof(GLfloat), colours, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Wireframe::generateSquare(glm::vec3 pos, float size) {
    GLfloat vertices[] = {
        size,  0.0f,   size, 
        size,  0.0f,   -size,
        size,  0.0f,   -size,
        -size, 0.0f,   -size,
        -size, 0.0f,   -size,
        -size, 0.0f,   size, 
        size,  0.0f,   size, 
        -size, 0.0f,   size
    };
    
    numPoints = 8;

    glGenBuffers(1, &mLinePoints);
    glBindBuffer(GL_ARRAY_BUFFER, mLinePoints);
    glBufferData(GL_ARRAY_BUFFER, numPoints*3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    GLfloat colours[] = {
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f
    };

    glGenBuffers(1, &mLineColour);
    glBindBuffer(GL_ARRAY_BUFFER, mLineColour);
    glBufferData(GL_ARRAY_BUFFER, numPoints*3 * sizeof(GLfloat), colours, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Wireframe::render(Camera* camera) {
    mShader->use();
    glm::mat4 modelMatrix(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
    glm::mat4 MVP = camera->projectionMatrix() * camera->viewMatrix() * modelMatrix;

    glUniformMatrix4fv(glGetUniformLocation(mShader->programID(), "mvp"),  1, GL_FALSE, glm::value_ptr(MVP));

    glBindBuffer (GL_ARRAY_BUFFER, mLinePoints);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);

    glBindBuffer (GL_ARRAY_BUFFER, mLineColour);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);

    glDrawArrays(GL_LINES, 0, numPoints);
}
