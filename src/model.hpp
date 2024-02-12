#pragma once

#include "glm/gtc/type_ptr.hpp"

/**
 * Structure of a vertex as the model loading code provides them.
 */
class ModelVertex {
    public:
        glm::vec3 position;
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        glm::vec3 normal;
        glm::vec2 uv;
};


/**
 * This function loads in a model file and then provides the data through a
 * number of callbacks. I am going to add more callbacks to this when I
 * implement animation.
 * @param filename path to the model file.
 * @param state is a pointer to something where you can store the state of your
 *        loading in or whatever.
 * @param nMeshes is called first and tells you the number of meshes in the
 *        file.
 * @param mesh is called for each mesh in the model file. It tells you the name,
 *        the root position, the number of vertices, and the provides a pointer
 *        to all of the vertices in the mesh. This pointer will be valid until
 *        the load model function finishes at which point it will dissappear
 *        into the mists of time.
 * @param primitive is called for each primitive in the most recently sent
 *        mesh. it tells you the material used, then the number of sides per
 *        face, then the number of indices, then a pointer to a list of indices
 *        into the previously sent vertex data. Again the list of indices lasts
 *        for the function call.
 */
void loadModel(
    char const *filename,
    void *state,
    void (*nMeshes)(uint16_t),
    void (*mesh)(char const *, glm::vec3, uint16_t, ModelVertex const *),
    void (*primitive)(uint8_t, uint8_t, uint16_t, uint16_t const *)
);