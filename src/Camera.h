#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
    Camera();

    glm::vec3 cameraPosition;
    glm::vec3 cameraRotation;

    void moveCamera(glm::vec3 delta);
    void update();

    inline glm::mat4 viewMatrix() const { return mViewMatrix; };
    inline glm::mat4 projectionMatrix() const { return mProjectionMatrix; };
    float zoom;

    void setViewPort(int width, int height);
private:
    float mAspectRatio;
    int mWidth, mHeight;

    glm::mat4 mViewMatrix;
    glm::mat4 mProjectionMatrix;

    glm::vec3 mOffset;
    glm::vec3 mUp;
    glm::vec3 mRight;
    glm::vec3 mDirection;
};

