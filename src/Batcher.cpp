#include "Batcher.hpp"

using namespace Util;

Batcher::TexturedBatch::TexturedBatch(
    Texture const &texture,
    int max,
    int *total
):
    texture(texture),
    max(max),
    total(total)
{
    verts = new Vert[max * 4];
    idxs = new uint16_t[max * 6];
    for (int i = 0; i < max; i++) {
        idxs[i * 6] = i;
        idxs[i * 6 + 1] = i + 1;
        idxs[i * 6 + 2] = i + 2;
        idxs[i * 6 + 3] = i;
        idxs[i * 6 + 4] = i + 2;
        idxs[i * 6 + 5] = i + 3;
    }
}

void Batcher::TexturedBatch::clear() {
    n = 0;
}

void Batcher::TexturedBatch::draw() const {
    texture.bind();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_UNSIGNED_SHORT, sizeof(Vert), (void*)(&verts[0].x));
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vert), (void*)(&verts[0].u));
    glDrawElements(GL_TRIANGLES, n * 6, GL_UNSIGNED_SHORT, idxs);
}

Batcher::BSprite::BSprite(Util::Rect bounds, TexturedBatch &batch):
    batch(batch),
    bounds(bounds)
{
}

void Batcher::BSprite::addCentered(glm::mat3 transform) const {

}

void Batcher::BSprite::add(Util::Rect dst) const {
    if (batch.n >= batch.max) {
        fprintf(stderr, "Texture batch max items reached\n");
        return;
    }
    // TODO: we need to maintain a global count of things drawn in order for
    //       the z ordering to work. Also we should add stuff to the list from
    //       the back to the front so that the things that appear in front are
    //       drawn first thus taking advantage of the z buffer.
    batch.verts[batch.n * 4].pos = dst.pos;
    batch.verts[batch.n * 4 + 1].pos = dst.r();
    batch.verts[batch.n * 4 + 2].pos = dst.tr();
    batch.verts[batch.n * 4 + 3].pos = dst.tl();
    batch.verts[batch.n * 4].uv = bounds.pos;
    batch.verts[batch.n * 4 + 1].uv = bounds.r();
    batch.verts[batch.n * 4 + 2].uv = bounds.tr();
    batch.verts[batch.n * 4 + 3].uv = bounds.tl();
}

Batcher::Batcher(int maxSprites, char const *filename) {
    TokenReader reader(filename);
    assert(reader.hasMore());
    int n = reader.nextInt();
}

void Batcher::render() {

}

void Batcher::clear() {
    for (int i = 0; i < nTextures; i++) batches[i].clear();
}