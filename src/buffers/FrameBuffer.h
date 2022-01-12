#pragma once
#include <glad/glad.h>
#include <stdio.h>


class FrameBuffer {
    public:
        FrameBuffer();
        ~FrameBuffer();

        void bind();
        void unbind();


    private:
        GLuint mFboId;
        GLuint mTextureId;

};
