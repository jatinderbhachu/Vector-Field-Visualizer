#include <Texture.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint Texture::texUnitCounter = 1;

Texture::Texture(){
    mTexture = -1;
}

Texture::~Texture(){ 
}

GLuint Texture::loadTexture(const char *file){
    int nrChannels;
    glCreateTextures(GL_TEXTURE_2D, 1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, this->mTexture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        //Create 2d mipmaps
        //glGenerateMipmap(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
        mTextureUnit = texUnitCounter;
        //glBindTextureUnit(mTextureUnit, mTexture);
        texUnitCounter++;
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("failed to load texture");
    }
    /* printf("texture dimensions: %d, %d, %d, %d\n", width, height, nrChannels, mTexture); */
    stbi_image_free(data);
    return mTexture;
}

GLuint Texture::createComputeTexture() {
    width = 512;
    height = 512;
    GLuint tex_output;
    glGenTextures(1, &mTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    mTextureUnit = texUnitCounter;
    texUnitCounter++;
    glBindImageTexture(0, mTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    return mTexture;
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, this->mTexture);
}

void Texture::bindTextureUnit() const {
    printf("");
    glBindTextureUnit(mTextureUnit, mTexture);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}
