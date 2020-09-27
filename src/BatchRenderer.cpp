#include "glm/trigonometric.hpp"
#include <BatchRenderer.h>

BatchRenderer::BatchRenderer() { init(); }

BatchRenderer::~BatchRenderer() {
    delete m_IBO;
    glDeleteBuffers(1, &m_VBO);
    delete[] m_BufferBase;
}

void BatchRenderer::init() {
    m_BufferBase = new VertexData[RendererVertexCount];

    glCreateVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glCreateBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, RendererVertexCount * sizeof(VertexData), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData),
            (const void *)offsetof(VertexData, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData),
            (const void *)offsetof(VertexData, colour));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
            (const void *)offsetof(VertexData, texCoord));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(VertexData),
            (const void *)offsetof(VertexData, texID));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint *indices = new GLuint[RendererIndicesCount];

    int offset = 0;
    for (int i = 0; i < RendererIndicesCount; i += 6) {
        indices[i] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;

        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;

        offset += 4;
    }

    m_IBO = new IndexBuffer(indices, RendererIndicesCount);
    QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
    QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
    QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

    glBindVertexArray(0);


    glCreateTextures(GL_TEXTURE_2D, 1, &m_WhiteTexture);
    glBindTexture(GL_TEXTURE_2D, m_WhiteTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    uint32_t color = 0xffffffff;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

}

void BatchRenderer::checkMaxVerts(){
    //printf("m_IndexCount: %i, max: %zu\n", m_IndexCount, RendererIndicesCount);
    if(m_IndexCount >= RendererIndicesCount){
        //printf("maxed out indices, flushing\n");

        end();

        flush();

        m_IndexCount = 0;
        m_BufferPtr = m_BufferBase;

        //begin();
    }
}

void BatchRenderer::begin() {
    m_IndexCount = 0;
    m_BufferPtr = m_BufferBase;
}

void BatchRenderer::end() {

    uint32_t size = (uint8_t*)m_BufferPtr - (uint8_t*)m_BufferBase;
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, m_BufferBase);

    flush();

}

void BatchRenderer::flush() {
    glBindVertexArray(m_VAO);
    m_IBO->bind();

    _viewMatrix = Camera::_viewMatrix;
    _projectionMatrix = Camera::_projectionMatrix;

    glm::mat4 modelMatrix(1.0f);

    glm::mat4 MVP = _projectionMatrix * _viewMatrix * modelMatrix;

    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(MVP));

    // glActiveTexture(GL_TEXTURE0);
    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, NULL);

    // Wireframe
    //glDisable(GL_BLEND);
    //glDrawElements(GL_LINES, m_IndexCount, GL_UNSIGNED_INT, NULL);
    //glEnable(GL_BLEND);

    m_IBO->unbind();
    glBindVertexArray(0);

    m_IndexCount = 0;
}

void BatchRenderer::submit(const Renderable *renderable) {
    checkMaxVerts();
    const glm::vec3 &position = renderable->getPosition();
    const glm::vec2 &size = renderable->getSize();
    // const glm::vec4& color = renderable->getColor();
    const glm::vec4 &color = {1.0f, 1.0f, 1.0f, 1.0f};
    ((Sprite *)renderable)->getTexture().bindTextureUnit();
    const float textureID = ((Sprite *)renderable)->getTexture().getTextureUnit();
    // printf("position %1.2f, %1.2f\n", position.x, position.y);
    // printf("size %1.2f, %1.2f\n", size.x, size.y);

    m_BufferPtr->position = {position.x, position.y, 0.0f};
    m_BufferPtr->colour = color;
    m_BufferPtr->texCoord = {0.0f, 0.0f};
    m_BufferPtr->texID = textureID;
    m_BufferPtr++;

    m_BufferPtr->position = {position.x, position.y + size.y, 0.0f};
    m_BufferPtr->colour = color;
    m_BufferPtr->texCoord = {0.0f, 1.0f};
    m_BufferPtr->texID = textureID;
    m_BufferPtr++;

    m_BufferPtr->position = glm::vec3(position.x + size.x, position.y + size.y, position.z);
    m_BufferPtr->colour = color;
    m_BufferPtr->texCoord = {1.0f, 1.0f};
    m_BufferPtr->texID = textureID;
    m_BufferPtr++;

    m_BufferPtr->position = glm::vec3(position.x + size.x, position.y, position.z);
    m_BufferPtr->colour = color;
    m_BufferPtr->texCoord = {1.0f, 0.0f};
    m_BufferPtr->texID = textureID;
    m_BufferPtr++;

    m_IndexCount += 6;
}

// TODO: generate vertices using for loop instead of hard coding them
void BatchRenderer::submit(const Sprite *sprite) {
    checkMaxVerts();
    const glm::vec3 &position = sprite->getPosition();
    const glm::vec2 &size = sprite->getSize();
    // const glm::vec4& color = renderable->getColor();
    const glm::vec4 &color = {1.0f, 1.0f, 1.0f, 1.0f};
    const float textureID = sprite->getTexture().getTextureUnit();
    const float rotation = sprite->getRotation();
    sprite->getTexture().bindTextureUnit();
    // printf("position %1.2f, %1.2f\n", position.x, position.y);
    // printf("size %1.2f, %1.2f\n", size.x, size.y);

    //constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f } };

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), {position.x, position.y, 0.0f})
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    
    m_BufferPtr->position = transform * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f );
    m_BufferPtr->colour = color;
    m_BufferPtr->texCoord = {0.0f, 0.0f};
    m_BufferPtr->texID = textureID;
    m_BufferPtr++;

    m_BufferPtr->position = transform * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);
    m_BufferPtr->colour = color;
    m_BufferPtr->texCoord = {0.0f, 1.0f};
    m_BufferPtr->texID = textureID;
    m_BufferPtr++;

    m_BufferPtr->position = transform * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
    m_BufferPtr->colour = color;
    m_BufferPtr->texCoord = {1.0f, 1.0f};
    m_BufferPtr->texID = textureID;
    m_BufferPtr++;

    m_BufferPtr->position = transform * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);
    m_BufferPtr->colour = color;
    m_BufferPtr->texCoord = {1.0f, 0.0f};
    m_BufferPtr->texID = textureID;
    m_BufferPtr++;

    m_IndexCount += 6;
}

void BatchRenderer::submit(const AnimatedSprite* sprite){
    checkMaxVerts();
    glm::mat4 modelMatrix(1.0f);
    int frame = sprite->getFrame();
    glm::vec3 position = sprite->getPosition();
    glm::vec2 size = sprite->getSize();
    const glm::vec4 &color = {1.0f, 1.0f, 1.0f, 1.0f};
    float x_uv = sprite->getXUV();
    float y_uv = sprite->getYUV();

    float rotation = sprite->getRotation();

    //texCoord = vec2((frameIndex + aTexCoord.x)*(x_uv), (aTexCoord.y)+(y_uv));
    const float textureID = sprite->getTexture()->getTextureUnit();
    sprite->getTexture()->bindTextureUnit();
    // printf("position %1.2f, %1.2f\n", position.x, position.y);
    // printf("size %1.2f, %1.2f\n", size.x, size.y);

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), {position.x, position.y, 0.0f})
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    m_BufferPtr->position = transform * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f );
    m_BufferPtr->colour = color;
    m_BufferPtr->texCoord = {(0.0f + frame) * x_uv, 0.0f*(y_uv)};
    m_BufferPtr->texID = textureID;
    m_BufferPtr++;

    m_BufferPtr->position = transform * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);
    m_BufferPtr->colour = color;
    m_BufferPtr->texCoord = {(0.0f + frame) * x_uv, 1.0f*(y_uv)};
    m_BufferPtr->texID = textureID;
    m_BufferPtr++;

    m_BufferPtr->position = transform * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
    m_BufferPtr->colour = color;
    m_BufferPtr->texCoord = {(1.0f + frame) * x_uv, 1.0f*(y_uv)};
    m_BufferPtr->texID = textureID;
    m_BufferPtr++;

    m_BufferPtr->position = transform * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);
    m_BufferPtr->colour = color;
    m_BufferPtr->texCoord = {(1.0f + frame) * x_uv, 0.0f*(y_uv)};
    m_BufferPtr->texID = textureID;
    m_BufferPtr++;

    m_IndexCount += 6;

}
