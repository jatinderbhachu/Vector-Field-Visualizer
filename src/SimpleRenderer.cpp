#include <SimpleRenderer.h>


SimpleRenderer::SimpleRenderer(){  }
SimpleRenderer::~SimpleRenderer(){  }

void SimpleRenderer::submit(const Renderable *sprite){
    mRenderQueue.push_back((StaticSprite*)sprite);
}

void SimpleRenderer::flush(){
    while(!mRenderQueue.empty()){
        _viewMatrix = Camera::_viewMatrix;
        _projectionMatrix = Camera::_projectionMatrix;

        const StaticSprite* sprite = mRenderQueue.front();

        sprite->getShader()->use();
        glActiveTexture(GL_TEXTURE0);
        /* printf("%u\n", sprite->getShader()->programID()); */

        glm::mat4 modelMatrix(1.0f);

        modelMatrix = glm::translate(modelMatrix, sprite->getPosition());
        //printf("(%f, %f)", sprite->getPosition().x, sprite->getPosition().y);
        modelMatrix = glm::rotate(modelMatrix, sprite->getRotation(), glm::vec3(0.0f, 0.0f, 1.0f));

        /* modelMatrix = glm::scale(modelMatrix, glm::vec3(50, 50, 0.0f)); */

        glm::mat4 MVP = _projectionMatrix * _viewMatrix * modelMatrix;
        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(MVP));

        sprite->getVAO()->bind();
        sprite->getIBO()->bind();
        sprite->getTexture().bindTextureUnit();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        sprite->getIBO()->unbind();
        sprite->getVAO()->unbind();
        mRenderQueue.pop_front();
    }
}

void SimpleRenderer::renderAnimSprite(const Camera *camera, const AnimatedSprite* sprite){
    _viewMatrix = camera->viewMatrix();
    _projectionMatrix = camera->projectionMatrix();

    sprite->getShader()->use();


    glm::mat4 modelMatrix(1.0f);

    modelMatrix = glm::translate(modelMatrix, sprite->getPosition());
    //printf("(%f, %f)", sprite->getPosition().x, sprite->getPosition().y);
    modelMatrix = glm::rotate(modelMatrix, sprite->getRotation(), glm::vec3(0.0f, 0.0f, 1.0f));

    /* modelMatrix = glm::scale(modelMatrix, glm::vec3(50, 50, 0.0f)); */

    glm::mat4 MVP = _projectionMatrix * _viewMatrix * modelMatrix;
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(MVP));
    glUniform1ui(1, sprite->getFrame()); // set frame
    glUniform1f(2, sprite->getXUV()); // set frame
    glUniform1f(3, sprite->getYUV()); // set frame

    sprite->getVAO()->bind();
    sprite->getIBO()->bind();
    sprite->getTexture()->bind();

    glDrawElements(GL_TRIANGLES, sprite->getIBO()->getCount(), GL_UNSIGNED_INT, 0);

    sprite->getVAO()->unbind();
    sprite->getIBO()->unbind();
}








