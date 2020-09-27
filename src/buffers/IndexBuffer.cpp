#include <buffers/IndexBuffer.h>

IndexBuffer::IndexBuffer(GLuint* data, GLsizei count){
    mCount = count;
    glGenBuffers(1, &mBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(GLuint), data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::IndexBuffer(){ }

IndexBuffer::~IndexBuffer(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferID);
    glDeleteBuffers(1, &mBufferID);
}

void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferID);

    /* printf("\n bind ibo: %u, id: %u", glGetError(), mBufferID); */
}

void IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* printf("\n unbind ibo: %u", mBufferID); */
}
