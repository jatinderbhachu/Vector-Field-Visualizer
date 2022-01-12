#pragma once

#include <glad/glad.h>
#include <stdio.h>

class IndexBuffer {
    public:
        IndexBuffer(GLuint* data, GLsizei count);
        IndexBuffer();
        ~IndexBuffer();

        void bind() const;
        void unbind() const;

        inline GLuint getCount() const { return mCount; };

    private:
        GLuint mBufferID;
        GLuint mCount;
};
