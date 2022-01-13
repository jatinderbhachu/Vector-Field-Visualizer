#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
    Camera();

    glm::vec3 mCameraPosition;

    void rotateCamera(glm::vec2 mouseDelta);
    void update();

    inline glm::mat4 viewMatrix() const { return mViewMatrix; };
    inline glm::mat4 projectionMatrix() const { return mProjectionMatrix; };

    void setViewPort(int width, int height);
private:
    int mWidth, mHeight;

    glm::mat4 mViewMatrix;
    glm::mat4 mProjectionMatrix;

    glm::vec3 mUp;
    glm::vec3 mRight;
    glm::vec3 mDirection;
    glm::vec3 mOrbitPos;
};

