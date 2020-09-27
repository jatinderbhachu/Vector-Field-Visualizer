#pragma once
#include "glm/geometric.hpp"
#define PI			3.1415926535897932384626433832795
#define HALF_PI		1.5707963267948966192313216916398
#define RAD_TO_DEG	57.295779513082320876798154814105

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <GLFW/glfw3.h>
#include <stdio.h>




class Camera
{
    public:
        Camera();
        ~Camera();

        static glm::vec3 cameraPosition;
        static glm::vec3 cameraRotation;

        void setPosition(glm::vec3);
        /// @brief tilt the Camera (look up/down)
        /// @param amount the amount to tilt
        /// @return void
        void tilt(float amount)		{ cameraRotation.x += amount; };
        /// @brief pan the Camera (look left/right)
        /// @param amount the amount to pan
        /// @return void
        void pan(float amount)		{ cameraRotation.y += amount; };
        /// @brief roll the Camera (around Z)
        /// @param amount the amount to roll in radians
        /// @return void
        void roll(float amount)		{ cameraRotation.z += amount; };
        /// @brief dolly the Camera (walk forward/backward)
        /// @param amount the amount to dolly
        /// @return void
        void dolly(float amount)	{ cameraPosition += (_direction * amount); };
        /// @brief track the Camera (walk sideways / strafe)
        /// @param amount the amount to track
        /// @return void
        void track(float amount)	{ cameraPosition += (_right * amount); };
        /// @brief boom the Camera (walk up/down)
        /// @param amount the amount to boom
        /// @return void
        void boom(float amount)		{ cameraPosition += (_up * amount); };

        void moveCamera(glm::vec3 delta) { 
            //printf("(%1.2f, %1.2f)\n", delta.x, delta.y);
            cameraPosition = glm::rotateY(cameraPosition, glm::radians(delta.x));
            cameraPosition = glm::rotate(cameraPosition, glm::radians(delta.y), glm::cross(cameraPosition, {0, 1, 0}));

        };

        static void scale(float amount) { cameraPosition *= amount; };

        void update();


        glm::mat4 viewMatrix() const { return _viewMatrix; };
        glm::mat4 projectionMatrix() const { return _projectionMatrix; };
        static float zoom;

        static void setViewPort(int width, int height);
        static int mWidth, mHeight;

        static glm::mat4 _viewMatrix;
        static glm::mat4 _projectionMatrix;
        static float mAspectRatio;

        static glm::vec3 _offset;
    private:
        glm::vec3 _up;
        glm::vec3 _right;
        glm::vec3 _direction;
};

