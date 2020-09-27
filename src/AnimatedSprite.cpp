#include <AnimatedSprite.h>

AnimatedSprite::AnimatedSprite(int framesInRow, int framesInColumn, float rotation, Texture* tex, Shader* shader) : Renderable(glm::vec3(0, 0, 0), glm::vec2(100, 100), glm::vec4(1, 1, 1, 1)), mTexture(tex), mRotation(rotation), mShader(shader)
{


    mVAO = new VertexArray();

    mPos = glm::vec3(0, 0, 0);

    glm::vec2 size = glm::vec2(mTexture->getWidth()/framesInRow, mTexture->getHeight()/framesInColumn);
    mSize = size;
    //printf("size (%1.2f, %1.2f)\n",size.x, size.y);
    

    xUV = size.x/mTexture->getWidth();
    yUV = size.y/mTexture->getHeight();

    /* printf("sizeX: %f, x: %f, y%f\n",size.x, xUV, yUV); */

    GLfloat vertices[] = {
        // positions 
         size.x/2,  size.y/2,
         size.x/2, -size.y/2,
        -size.x/2, -size.y/2,
        -size.x/2,  size.y/2
    };

    GLfloat texCoords[] = {
        1.0f, 1.0f, // top right
        1.0f, 0.0f, // bottom right
        0.0f, 0.0f, // bottom left
        0.0f, 1.0f  // top left 
    };


    mVAO->addBuffers(new Buffer(vertices, 4 * 2, 2), 0);
    mVAO->addBuffers(new Buffer(texCoords, 4 * 2, 2), 1);

    GLuint indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    mIBO = new IndexBuffer(indices, 6);

    mMaxFrames = framesInRow*framesInColumn;

}

AnimatedSprite::~AnimatedSprite() { 
    delete mVAO;
    delete mIBO;
}

void AnimatedSprite::setFrameRate(int fps){
    mFrameRate = (float)fps;
    mFrameTime = 1.0/mFrameRate;
}

void AnimatedSprite::update(float deltaTime){
    if(mTimer >= mFrameTime){
        mCurrFrame++;
        mTimer = 0.0f;
    }

    if(mCurrFrame == mMaxFrames)
        mCurrFrame = 0;

    // update the time of this sprite
    mTimer += deltaTime;
    /* printf("FrameTime: %.3f Timer: %.3f\n", mFrameTime, mTimer); */
}

