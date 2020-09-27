#include <Camera.h>

int Camera::mWidth = 1280;
int Camera::mHeight = 720;
float Camera::zoom = 1.0f;

//glm::vec3 Camera::_offset = glm::vec3(mWidth/2, mHeight/2, 0);
glm::vec3 Camera::_offset = glm::vec3(10, 10, 10);

float Camera::mAspectRatio = (float)mWidth / (float)mHeight;
glm::mat4 Camera::_projectionMatrix = glm::perspective( glm::radians(90.0f), (float)mWidth/(float)mHeight, 0.0f, 100.0f);
glm::mat4 Camera::_viewMatrix = glm::mat4(1.0f);
glm::vec3 Camera::cameraPosition = {0, 0, 0};
glm::vec3 Camera::cameraRotation = {0, 0, 0};

Camera::Camera()
{
    cameraPosition = glm::vec3( 0, 0, 1 );
    cameraRotation = glm::vec3( 0, PI, 0 );

    _direction = glm::vec3(0, 0, -5);
    _up = glm::vec3(0, 1, 0);
    _right = glm::vec3(1, 0, 0);

    // to compensate for the camera being not in the correct position

    // Initial position : on +Z
    // Unintuitively, this is the centre of the screen, so add _offset
    cameraPosition = glm::vec3(0, 0, 0) + _offset;


    /* printf("%f", mAspectRatio); */

    /* _projectionMatrix = glm::ortho( -mAspectRatio*zoom, mAspectRatio*zoom, -mAspectRatio*zoom, mAspectRatio*zoom, -1.0f, 1.0f); */

    // Disable depth test (we don't need it in 2D)
    //glDisable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    /* glDepthFunc(GL_LESS); */
}

Camera::~Camera(){  }


void Camera::setViewPort(int width, int height){
    mWidth = width;
    mHeight = height;
    //_offset = glm::vec3(mWidth/2, mHeight/2, 0);
    //mAspectRatio = (float)mWidth / (float)mHeight;
    _projectionMatrix = glm::ortho( -mWidth/zoom, mWidth/zoom, -mHeight/zoom, mHeight/zoom, -1.0f, 100.0f);
}

void Camera::update(){
// Direction vector: Spherical coordinates to Cartesian coordinates conversion
    
    //_projectionMatrix = glm::ortho( -mWidth/zoom, mWidth/zoom, -mHeight/zoom, mHeight/zoom, -1.0f, 1.0f);
    _projectionMatrix = glm::perspective(glm::radians(90.0f), (float)mWidth/(float)mHeight, 0.0f, 100.0f);
    /* _projectionMatrix = glm::ortho( -mAspectRatio*zoom, mAspectRatio*zoom, -mAspectRatio*zoom, mAspectRatio*zoom, -1.0f, 1.0f); */

    _direction = glm::vec3(
            cos(cameraRotation.x) * sin(cameraRotation.y),
            sin(cameraRotation.x),
            cos(cameraRotation.x) * cos(cameraRotation.y)
            );
    //_direction = glm::vec3(0, 0, -5);

    // Right vector
    _right = glm::vec3(
            sin(cameraRotation.y - HALF_PI),
            0,
            cos(cameraRotation.y - HALF_PI)
            );
    _right = glm::rotateZ(_right, cameraRotation.z); // rotateZ() in Degrees
    //_right = glm::vec3(1, 0, 0);

    // Up vector
    _up = glm::cross( _right, _direction );
    //_up = glm::vec3(0, 1, 0);

    glm::vec3 pos = glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z);
    glm::vec3 offset = glm::vec3(_offset.x, _offset.y, _offset.z);

    // View matrix
    _viewMatrix = glm::lookAt(
            pos, // Camera is at (xpos,ypos,1), in World Space
            //pos - offset + _direction, // and looks towards Z
            {0, 0, 0},
            _up  // Head is up
            );
}
