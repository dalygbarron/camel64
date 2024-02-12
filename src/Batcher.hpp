#pragma once

#include "Texture.hpp"
#include "Util.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Util.hpp"

/**
 * The classic old batcher class that draws a whole bunch of sub images from a
 * single texture in minimal draw calls.
 */
class Batcher {
    private:
        struct __attribute__((packed)) Vert {
            GLshort x;
            GLshort y;
            GLshort z;
            GLfloat u;
            GLfloat v;
            uint8_t r;
            uint8_t g;
            uint8_t b;
        };

        /**
         * A batching thingy that can only be for a given texture at a given
         * time.
         */
        class TexturedBatch {
            public:
                int const max;
                Texture const *texture = NULL;
                Vert *vertices = NULL;
                uint16_t *indices = NULL;
                int n = 0;

                TexturedBatch(int max);

                /**
                 * Clears the count of sprites and the texture used.
                 */
                void clear();

                /**
                 * Draws it to the screen.
                 */
                void draw() const;
        };

        int const maxSprites;
        int const maxTextures;
        TexturedBatch *batches;

    public:
        /**
         * Creates the batcher.
         * @param maxSprites is the maximum rectangles it can draw for any
         *        particular texture.
         * @param maxTextures is the maximum textures it can draw with in any
         *        given batch.
         */
        Batcher(int maxSprites, int maxTextures);

        /**
         * Adds a rectangle to the batch.
         * @param dst is where it will appear on the screen.
         * @param src is where in the texture it gets image data from.
         * @param colour is a colour modifier to add.
         */
        void add(Util::Rect dst, Util::Rect src, color_t colour = Util::WHITE);

        /**
         * Draws all the currently batched stuff onto the screen.
         */
        void render();

        /**
         * Clears what is presently batched.
         */
        void clear();
};