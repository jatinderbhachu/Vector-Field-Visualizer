#pragma once
#include "glad/glad.h"
#include <glm/glm.hpp>

class Camera;
class Shader;

class Wireframe {

    public:
        Wireframe(Shader* shader);

        void generateLine(glm::vec3 p1, glm::vec3 p2);
        void generateSquare(glm::vec3 pos, float size);
        void generateAxis(float size);

        void render(Camera* camera);

    private:
        Shader* mShader;
        GLuint mLinePoints, mLineColour;
        unsigned int numPoints;
};
