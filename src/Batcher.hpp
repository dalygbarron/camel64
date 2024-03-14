#pragma once

#include "Texture.hpp"
#include "Util.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Util.hpp"
#include "hashmap.h"

/**
 * The classic old batcher class that draws a whole bunch of sub images from a
 * single texture in minimal draw calls.
 */
class Batcher {
    private:
        struct __attribute__((packed)) Vert {
            uint16_t x;
            uint16_t y;
            uint16_t z;
            float u;
            float v;
        };

        /**
         * A batching thingy that can only be for a given texture at a given
         * time.
         */
        class TexturedBatch {
            public:
                int max;
                Texture const *texture;
                mutable Vert *verts = NULL;
                mutable uint16_t *idxs = NULL;
                int *total;
                mutable int n = 0;

                /**
                 * Default constructor. Creates the batch such that it can't
                 * actually be used because it's max is 0.
                 */
                TexturedBatch();

                /**
                 * Creates the sub batch.
                 * @param texture is the texutre t
                */
                TexturedBatch(Texture const *texture, int max, int *total);

                /**
                 * Sets the positioning and uv of the vertex at the given index.
                 * @param n is the index of the vertex.
                 * @param pos is the screen position of the vertex.
                 * @param z is the draw order of the vertex.
                 * @param uv is the texture mapping position of the vertex.
                 */
                void setVert(int n, glm::vec2 pos, float z, glm::vec2 uv);

                /**
                 * Clears the count of sprites and the texture used.
                 */
                void clear();

                /**
                 * Draws it to the screen.
                 */
                void render() const;
        };

        struct hashmap *sprites;
        TexturedBatch *batches;
        int nTextures;
        int nFrame;

    public:
        /**
         * Represents a given sub image that can be drawn by the batcher. Is
         * permanently associated with the batcher that spawned it and the
         * texture it was created to draw from.
         */
        class BSprite {
            private:
                TexturedBatch &batch;
            
            public:
                char const * const name;
                Util::Rect const bounds;

                /**
                 * Creates the little fucker.
                 * @param name is the name of the sprite and it's key in the
                 *        batcher's hashmap of sprites.
                 * @param bounds is the position of the sprite in it's sheet and
                 *               it's dimensions.
                 * @param batch is the specific texture batch it works with.
                 */
                BSprite(
                    char const *name,
                    Util::Rect bounds,
                    TexturedBatch &batch
                );

                /**
                 * Adds a rectangle to be drawn, but rather than providing a
                 * rectangle that it will fill on the screen, you provide a 3x3
                 * transformation matrix which sets it's position, rotation, and
                 * scale, with the baseline being based on the source rectangle.
                 * @param transform is the transformation matrix.
                 */
                void addCentered(glm::mat3 transform) const;

                /**
                 * Adds a rectangle to be drawn on the next frame.
                 * @param dst is where to draw it.
                 * @param colour colour info to add to all vertices.
                 */
                void add(Util::Rect dst) const;
        };

        /**
         * Creates the batcher.
         * @param maxSprites is the maximum rectangles it can draw for any
         *        particular texture.
         * @param filename is the file from which it loads the individual sprite
         *        definitions and the textures that it needs.
         */
        Batcher(int maxSprites, char const *filename);

        /**
         * Gets a sprite from the list of sprites that this batcher has.
         * @param name is the name by which the sprite is known.
         * @return pointer to the sprite or NULL if it's not there.
         */
        BSprite const *getSprite(char const *name) const;

        /**
         * Draws all the currently batched stuff onto the screen.
         */
        void render();

        /**
         * Clears what is presently batched.
         */
        void clear();
};