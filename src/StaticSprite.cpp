#include <StaticSprite.h>

StaticSprite::StaticSprite(glm::vec3 pos, glm::vec2 size, Texture tex, Shader* shader) : Renderable(pos, size, glm::vec4(1, 1, 1, 1)), mPos(pos), mTexture(tex), mShader(shader)
{

    mVAO = new VertexArray();

    /* ANGRY
    GLfloat vertices[] = {
        // positions 
        size.x/2,  0.0f,
        size.x/2, -size.y/2,
        -size.x/2, -size.y/2,
        -size.x/2,  size.y/2
    };
    */

    GLfloat vertices[] = {
        // positions 
         size.x/2,  size.y/2, 0.0f,
         size.x/2, -size.y/2, 0.0f,
        -size.x/2, -size.y/2, 0.0f,
        -size.x/2,  size.y/2, 0.0f
    };

    GLfloat texCoords[] = {
        1.0f, 1.0f, // top right
        1.0f, 0.0f, // bottom right
        0.0f, 0.0f, // bottom left
        0.0f, 1.0f  // top left 
    };

    GLfloat colours[] = {
        1.0f, 1.0f, 1.0f, 1.0f
    };

    GLfloat textureID[] = {1.0f};


    mVAO->addBuffers(new Buffer(vertices, 4 * 3, 3), 0);
    mVAO->addBuffers(new Buffer(colours, 4, 4), 1);
    mVAO->addBuffers(new Buffer(texCoords, 4 * 2, 2), 2);
    mVAO->addBuffers(new Buffer(textureID, 1, 1), 3);


    GLuint indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    mIBO = new IndexBuffer(indices, 6);

}

StaticSprite::~StaticSprite() { 
    delete mVAO;
    delete mIBO;
}

void StaticSprite::setTexture(Texture& tex){
    mTexture = tex;
    mSize = mTexture.getSize();
}

