#include <buffers/VertexArray.h>

VertexArray::VertexArray(){ 
    if (glGenVertexArrays == NULL)
    {
        // chances are you don't have this feature...
        printf("\ninit vao%d", mArrayID);
    }
    glGenVertexArrays(1, &mArrayID);
}

void VertexArray::setup(){ 
    glGenVertexArrays(1, &mArrayID);
}

VertexArray::~VertexArray(){
    for(int i = 0; i < mBuffers.size(); i++)
        delete mBuffers[i];
}

void VertexArray::addBuffers(Buffer *buffer, GLuint index){
    bind();
    buffer->bind();

    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, buffer->getComponentCount(), GL_FLOAT, GL_FALSE, 0, 0);

    buffer->unbind();
    unbind();

    /* printf("\n bindvao: %u, arrayID: %u", glGetError(), mArrayID); */
}

void VertexArray::bind() const {
    glBindVertexArray(mArrayID);
    /* printf("\n bindvao: %u, arrayID: %d", glGetError(), mArrayID); */
}

void VertexArray::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* printf("\n unbind: %u", mArrayID); */

}
