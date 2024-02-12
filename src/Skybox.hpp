#pragma once

#include "Util.hpp"
#include "Texture.hpp"

/**
 * Yeah baby this thing manages a bunch of textures and has a nice beautiful
 * rendering of a skybox, you just have to pass in the camera's rotations and it
 * figures it out for you nicely and when you are done you can delete it or
 * let it go out of scope because it probably uses a lot of memory.
 */
class Skybox {
    public:
        static uint16_t const EXPECTED_WIDTH = 128;
        static uint16_t const EXPECTED_HEIGHT = 192;
        enum TextureIndex: uint16_t {
            SKY_FRONT_TOP_LEFT = 0,
            SKY_FRONT_TOP_RIGHT,
            SKY_FRONT_BOTTOM_LEFT,
            SKY_FRONT_BOTTOM_RIGHT,
            SKY_RIGHT_TOP_LEFT,
            SKY_RIGHT_TOP_RIGHT,
            SKY_RIGHT_BOTTOM_LEFT,
            SKY_RIGHT_BOTTOM_RIGHT,
            SKY_BACK_TOP_LEFT,
            SKY_BACK_TOP_RIGHT,
            SKY_BACK_BOTTOM_LEFT,
            SKY_BACK_BOTTOM_RIGHT,
            SKY_LEFT_TOP_LEFT,
            SKY_LEFT_TOP_RIGHT,
            SKY_LEFT_BOTTOM_LEFT,
            SKY_LEFT_BOTTOM_RIGHT,
            SKY_TOP_TOP_LEFT,
            SKY_TOP_TOP_RIGHT,
            SKY_TOP_BOTTOM_LEFT,
            SKY_TOP_BOTTOM_RIGHT,
            SKY_BOTTOM_TOP_LEFT,
            SKY_BOTTOM_TOP_RIGHT,
            SKY_BOTTOM_BOTTOM_LEFT,
            SKY_BOTTOM_BOTTOM_RIGHT,
            SKY_N
        };

    private:
        bool initialised = false;
        GLuint textures[SKY_N];
        GLuint drawLists;

    public:
        /**
         * Frees all the textures and stuff if they are present.
         */
        ~Skybox();

        /**
         * Initialises the skybox based on an image that should contain a bunch
         * of other images in a special order.
         */
        void init(char const *filename);

        /**
         * Frees the current content.
         */
        void free();

        /**
         * Renders the skybox but determines which faces to render based on the
         * camera direction, since there is a lot of texture switching I figure
         * we should preemptively figure out which faces not to draw.
         * @param cameraDirection is the direction of the camera for culling
         *        unnecessary faces/texture loads. Shouldn't need to be
         *        normalised but if I am wrong about that forgive me.
         */
        void render(glm::vec3 cameraDirection) const;
};