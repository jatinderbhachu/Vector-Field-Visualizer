/*
#include <Renderer.h>

Renderer::Renderer() { 
}

Renderer::~Renderer() {  }

void Renderer::renderSprite(Sprite sprite){
    _viewMatrix = mCamera->viewMatrix();
    _projectionMatrix = mCamera->projectionMatrix();

    _defaultShader->use();
    glUniform1i(glGetUniformLocation(_defaultShader->programID(), "myTexture"), 0);


    //printf("\n useprogram: %u, id: %d", glGetError(), _defaultShader->programID());

    // printf("\nprogramID%u", _defaultShader->programID());

    glm::mat4 modelMatrix(1.0f);

    modelMatrix = glm::translate(modelMatrix, glm::vec3(sprite.getPos(), 0.0f));
    modelMatrix = glm::rotate(modelMatrix, sprite.getRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::rotate(modelMatrix, rotateTest, glm::vec3(0.0f, 0.0f, 1.0f));

    modelMatrix = glm::scale(modelMatrix, glm::vec3(50, 50, 0.0f));

    glm::mat4 MVP = _projectionMatrix * _viewMatrix * modelMatrix;
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(MVP));



    glActiveTexture(GL_TEXTURE0);
    sprite.bind();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

    sprite.unbind();

}

*/
