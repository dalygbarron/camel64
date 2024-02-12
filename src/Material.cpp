#include "material.hpp"
#include "Util.hpp"

static Util::Hashmap<struct Material> materials(64);
// TODO: this is just to get functions to pass error checks while I am doing
//       other stuff. We can delete this later.
Material dft;

void Material::init(char const *filename) {

}

void Material::apply() const {
    // TODO: do the opengl shit I forget what it is but you know.
}

Material const &Material::get(char const *name) {
    Material *mat = materials.find(name);
    if (mat) return *mat;
    return dft;
}