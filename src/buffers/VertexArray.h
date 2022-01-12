#pragma once

#include <glad/glad.h>
#include <buffers/Buffer.h>
#include <vector>

class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        void addBuffers(Buffer* buffer, GLuint count);
        void setup();

        void bind() const;
        void unbind() const;

    private:
        GLuint mArrayID;
        std::vector<Buffer*> mBuffers;
};
