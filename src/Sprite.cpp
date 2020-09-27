#include <Sprite.h>

Sprite::Sprite(glm::vec3 pos, glm::vec2 size, float rotation, glm::vec4 color, Texture tex) : Renderable(pos, size, color), mTexture(tex), mRotation(rotation){ }
