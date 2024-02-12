#include "model.hpp"
#include <libdragon.h>


void loadModel(
    char const *filename,
    void *state,
    void (*nMeshes)(uint16_t),
    void (*mesh)(char const *, glm::vec3, uint16_t, ModelVertex const *),
    void (*primitive)(uint8_t, uint8_t, uint16_t, uint16_t const *)
) {
    //void *data = asset_load(filename, NULL);

}