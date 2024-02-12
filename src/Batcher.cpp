#include "Batcher.hpp"

using namespace Util;

Batcher::TexturedBatch::TexturedBatch(int max): max(max) {
    vertices = new Vert[max * 4];
    indices = new uint16_t[max * 6];
    for (int i = 0; i < max; i++) {
        indices[i * 6] = i;
        indices[i * 6 + 1] = i + 1;
        indices[i * 6 + 2] = i + 2;
        indices[i * 6 + 3] = i;
        indices[i * 6 + 4] = i + 2;
        indices[i * 6 + 5] = i + 3;
    }
}

void Batcher::TexturedBatch::clear() {
    n = 0;
    texture = NULL;
}

void Batcher::TexturedBatch::draw() const {
    texture->bind();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_SHORT, sizeof(Vert), (void*)(&vertices[0].x));
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vert), (void*)(&vertices[0].u));
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(Vert), (void*)(&vertices[0].r));
    glDrawElements(GL_TRIANGLES, n * 6, GL_UNSIGNED_SHORT, indices);
}

Batcher::Batcher(int maxSprites, int maxTextures):
    maxSprites(maxSprites),
    maxTextures(maxTextures)
{
}

void Batcher::add(Rect dst, Rect src, color_t colour) {
    // if (n >= max) {
    //     debugf("Batcher max of %d exceeded", max);
    //     return;
    // }
    // vertices[n * 4].pos = dst.pos;
    // vertices[n * 4 + 1].pos = dst.r();
    // vertices[n * 4 + 2].pos = dst.tr();
    // vertices[n * 4 + 3].pos = dst.tl();
    // vertices[n * 4].uv = src.pos;
    // vertices[n * 4 + 1].uv = src.r();
    // vertices[n * 4 + 2].uv = src.tr();
    // vertices[n * 4 + 3].uv = src.tl();
    // for (int i = 0; i < 4; i++) vertices[n * 4 + i].colour = colour;
    // n++;
}

void Batcher::render() {
    
}

void Batcher::clear() {
    for (int i = 0; i < maxTextures; i++) batches[i].clear();
}