#pragma once

#include <GL/gl.h>
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

/**
 * A thing that can exist in three dimensions and all that kinda shit. It stores
 * it's transformation info in a fairly easy to read and use format, but then
 * it has also got a transformation matrix with a dirty flag so it can only be
 * updated when shit has changed and it is needed.
 */
class Spatial {
    private:
        mutable bool invDirty = false;
        mutable bool dirty = false;
        mutable glm::mat4 invMatrix = glm::mat4(1);
        mutable glm::mat4 matrix = glm::mat4(1);
        glm::vec3 position = glm::vec3(0);
        glm::vec3 rotation = glm::vec3(0);
        glm::vec3 scale = glm::vec3(1);

    public:
        /**
         * Gets the spatial's transformation matrix after updating it if needed.
         * @return up to date transformation matrix.
         */
        glm::mat4 const &getMatrix() const;

        /**
         * Gets the spatial's inverse transformation matrix after updating it if
         * necessary.
         * @return up to date inverse transformation matrix.
         */
        glm::mat4 const &getInvMatrix() const;

        /**
         * Gives you the position.
         * @return the spatial's position.
         */
        glm::vec3 getPosition() const;

        /**
         * Sets the position.
         * @param position the thing's new position.
         */
        void setPosition(glm::vec3 position);

        /**
         * Gives you the thing's rotation on each axis.
         * @return a vector with all the rotations.
         */
        glm::vec3 getRotation() const;

        /**
         * Sets the thingy's rotation on each axis. Keep in mind that they are
         * interpreted in ZYX order.
         * @param rotation is the vector of rotations.
         */
        void setRotation(glm::vec3 rotation);

        /**
         * Gives you the spatial's current scale setting in each dimension.
         * @return scale.
         */
        glm::vec3 getScale() const;

        /**
         * Lets you change the spatial's scale in each dimension.
         * @param scale the scale to set.
         */
        void setScale(glm::vec3 scale);
};