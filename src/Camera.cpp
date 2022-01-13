#include "Camera.h"
#include <iostream>
#include <glm/gtx/rotate_vector.hpp>

Camera::Camera() 
    : mCameraPosition(5.0f, 2.0f, -1.0f), 
    mDirection(0.0f), 
    mUp(0.0f, 1.0f, 0.0f), 
    mRight(0.0f), 
    mOrbitPos(0.0f) {
}

void Camera::rotateCamera(glm::vec2 mouseDelta) { 
    mCameraPosition = glm::rotateY(mCameraPosition, glm::radians(mouseDelta.x));
    mCameraPosition = glm::rotate(mCameraPosition, glm::radians(mouseDelta.y), glm::cross(mCameraPosition, {0, 1, 0}));
}


void Camera::setViewPort(int width, int height){
    mWidth = width;
    mHeight = height;
    update();
}

void Camera::update(){
    mProjectionMatrix = glm::perspective(glm::radians(90.0f), (float)mWidth/(float)mHeight, 1.0f, 5000.0f);

    mRight = glm::transpose(mViewMatrix)[0];
    mDirection = -glm::normalize(mCameraPosition - mOrbitPos);
    mUp = glm::normalize(glm::cross(mRight, mDirection));

    glm::vec3 pos = {mCameraPosition.x, mCameraPosition.y, mCameraPosition.z};

    mViewMatrix = glm::lookAt(
            pos,
            mOrbitPos,
            {0, 1, 0}
            );
}
