#pragma once

#include <glad/glad.h>

#include <stdio.h>

class Buffer {
    public:
        Buffer(GLfloat* data, GLsizei count, GLuint componentCount);
        ~Buffer();

        void bind() const;
        void unbind() const;

        inline GLuint getComponentCount() const { return mComponentCount; };

    private:
        GLuint mBufferID;
        GLuint mComponentCount;
};
