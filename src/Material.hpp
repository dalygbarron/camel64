#pragma once

#include <libdragon.h>
#include <GL/gl.h>

class Material {
    private:
        GLuint texture;

    public:
        /**
         * Initialises the material off the data in a file. If it screws up then
         * it just fails an assertion and nukes the whole program.
         * @param filename is the name of the file.
         */
        void init(char const *filename);

        /**
         * Activates the material in the current opengl context.
         */
        void apply() const;

        /**
         * Gets a material that has either been cached, or loads it and caches
         * it for your enjoyment.
         * @param name is the name of the material, which is like the file name
         *        but without the folder or extension.
         * @return the material you wanted if it can be found, or a default
         *         material is returned.
         */
        static Material const &get(char const *name);
};
