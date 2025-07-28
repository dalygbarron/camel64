#pragma once

#include "Util.hpp"
#include <GL/gl.h>
#include "glm/gtc/type_ptr.hpp"

class Texture {
    private:
        GLuint id = 0;
        glm::uvec2 size;
        sprite_t *sprite;

    public:
        /**
         * Creates the texture.
         * @param filename is the file of the texture.
         */
        Texture(char const *filename);

        /**
         * Just frees the texture when it goes out of scope.
         */
        ~Texture();

        /**
         * Binds the texture if there is something loaded, otherwise it will
         * write an error somewhere then do nothing.
         */
        void bind() const;

        /**
         * Tells you the size of the texture in pixels.
         * @return the size as a 2d unsigned vector.
         */
        glm::uvec2 getSize() const;
};