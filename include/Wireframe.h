#pragma once
#include <Renderable.h>
#include <Shader.h>
#include <Camera.h>
#include <buffers/VertexArray.h>
#include <buffers/Buffer.h>

class Wireframe {

    public:
        Wireframe(Shader* shader);

        void generateLine(glm::vec3 p1, glm::vec3 p2);
        void generateSquare(glm::vec3 pos, float size);
        void generateAxis(float size);

        void render();

    private:

        Shader* mShader;

        GLuint mLinePoints, mLineColour;

        unsigned int numPoints;




};
