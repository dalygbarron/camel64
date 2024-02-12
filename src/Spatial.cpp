#include "Spatial.hpp"
#include "glm/ext/matrix_transform.hpp"

glm::mat4 const &Spatial::getMatrix() const {
    if (dirty) {
        matrix = glm::mat4(1);
        matrix = glm::translate(matrix, position);
        matrix = glm::rotate(matrix, rotation.z, glm::vec3(0, 0, 1));
        matrix = glm::rotate(matrix, rotation.y, glm::vec3(0, 1, 0));
        matrix = glm::rotate(matrix, rotation.x, glm::vec3(1, 0, 0));
        matrix = glm::scale(matrix, scale);
        dirty = false;
    }
    return matrix;
}

glm::mat4 const &Spatial::getInvMatrix() const {
    if (invDirty) {
        invMatrix = glm::inverse(getMatrix());
        invDirty = false;
    }
    return invMatrix;
}

glm::vec3 Spatial::getPosition() const {
    return position;
}

void Spatial::setPosition(glm::vec3 position) {
    this->position = position;
    dirty = true;
    invDirty = true;
}

glm::vec3 Spatial::getRotation() const {
    return rotation;
}

void Spatial::setRotation(glm::vec3 rotation) {
    this->rotation = rotation;
    dirty = true;
    invDirty = true;
}

glm::vec3 Spatial::getScale() const {
    return scale;
}

void Spatial::setScale(glm::vec3 scale) {
    this->scale = scale;
    dirty = true;
    invDirty = true;
}