#include <buffers/FrameBuffer.h>


FrameBuffer::FrameBuffer(){
    glGenFramebuffers(1, &mFboId);

    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, mFboId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureId, 0); 
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

FrameBuffer::~FrameBuffer(){ 
    glDeleteFramebuffers(1, &mFboId);
}

void FrameBuffer::bind(){
    glBindFramebuffer(GL_FRAMEBUFFER, mFboId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureId, 0);
    if( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("\n[error] FRAMEBUFFER IS NOT COMPLETE");
}


void FrameBuffer::unbind(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
