#pragma once
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <vector>

class Camera;
class Shader;

class Wireframe {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 color;
    };

public:
    void generateLine(glm::vec3 p1, glm::vec3 p2);
    void generateSquare(glm::vec3 pos, float size);
    void generateAxis(float size);

    void render(Camera* camera);
private:

    void generateBuffers(std::vector<float>& vertices, std::vector<uint32_t>& indices);
    GLuint mVertexBuffer;
    GLuint mIndexBuffer;
    GLuint mVAO;
    uint32_t mNumIndices;
};
