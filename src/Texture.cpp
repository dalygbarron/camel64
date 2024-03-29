#include "Texture.hpp"
#include <libdragon.h>

Texture::Texture(char const *filename) {
    sprite_t *sprite = sprite_load(filename);
    size.x = sprite->width;
    size.y = sprite->height;
    glGenTextures(1, &id);
    assert(id != 0);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSpriteTextureN64(GL_TEXTURE_2D, sprite, NULL);
    // TODO: looks like we are leaking memory on the sprite??
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

void Texture::bind() const {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, id);
}

glm::uvec2 Texture::getSize() const {
    return size;
}