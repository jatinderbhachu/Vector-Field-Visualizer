#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>


class Texture {
    public:
        Texture();
        ~Texture();

        static GLuint texUnitCounter;

        void bind() const;
        void unbind() const;
        int getWidth() const { return width; };
        int getHeight() const { return height; };
        glm::vec2 getSize() const { return glm::vec2(width, height); };
        void bindTextureUnit() const;

        GLuint loadTexture(const char *file);
        GLuint createComputeTexture();
        GLuint getTextureUnit() const { return mTextureUnit; };
        GLuint mTexture;

    private:
        int width, height;
        GLuint mTextureUnit;

};

