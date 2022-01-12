#include "Camera.h"
#include <glm/gtx/rotate_vector.hpp>

Camera::Camera() {
    cameraPosition = glm::vec3(10.0f);
    cameraRotation = glm::vec3( 0, glm::pi<float>(), 0 );

    mDirection = glm::vec3(0, 0, -5);
    mUp = glm::vec3(0, 1, 0);
    mRight = glm::vec3(1, 0, 0);
}

void Camera::moveCamera(glm::vec3 delta) { 
    cameraPosition = glm::rotateY(cameraPosition, glm::radians(delta.x));
    cameraPosition = glm::rotate(cameraPosition, glm::radians(delta.y), glm::cross(cameraPosition, {0, 1, 0}));
}


void Camera::setViewPort(int width, int height){
    mWidth = width;
    mHeight = height;
    update();
}

void Camera::update(){
    mProjectionMatrix = glm::perspective(glm::radians(90.0f), (float)mWidth/(float)mHeight, 1.0f, 5000.0f);

    mDirection = glm::vec3(
            cos(cameraRotation.x) * sin(cameraRotation.y),
            sin(cameraRotation.x),
            cos(cameraRotation.x) * cos(cameraRotation.y)
            );

    // Right vector
    mRight = glm::vec3(
            sin(cameraRotation.y - glm::pi<float>()/2.0f),
            0,
            cos(cameraRotation.y - glm::pi<float>()/2.0f)
            );
    mRight = glm::rotateZ(mRight, cameraRotation.z);

    mUp = glm::cross( mRight, mDirection );

    glm::vec3 pos = {cameraPosition.x, cameraPosition.y, cameraPosition.z};

    mViewMatrix = glm::lookAt(
            pos,
            {0, 0, 0},
            mUp
            );
}
