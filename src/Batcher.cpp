#include "Batcher.hpp"

using namespace Util;

int spriteCompare(void const *a, void const *b, void *udata) {
    return strcmp(
        ((Batcher::BSprite const *)a)->name,
        ((Batcher::BSprite const *)b)->name
    );
}

uint64_t spriteHash(const void *item, uint64_t seed0, uint64_t seed1) {
    char const *name = ((Batcher::BSprite const *)item)->name;
    uint64_t hash = 0;
    for(int i = 0; name[i]; i++) {
        char c = name[i];
        hash += c;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

Batcher::TexturedBatch::TexturedBatch() {
    
}

Batcher::TexturedBatch::TexturedBatch(
    Texture const *texture,
    int max,
    int *total
):
    max(max),
    texture(texture),
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

void Batcher::TexturedBatch::setVert(
    int n,
    glm::vec2 pos,
    float z,
    glm::vec2 uv
) {
    assert(n < max * 4);
    verts[n].x = pos.x;
    verts[n].y = pos.y;
    verts[n].z = z;
    verts[n].u = uv.x;
    verts[n].v = uv.y;
}

void Batcher::TexturedBatch::render() const {
    if (n == 0) return;
    texture->bind();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_UNSIGNED_SHORT, sizeof(Vert), (void*)(&verts[0].x));
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vert), (void*)(&verts[0].u));
    glDrawElements(GL_TRIANGLES, n * 6, GL_UNSIGNED_SHORT, idxs);
}

Batcher::BSprite::BSprite(
    char const *name,
    Util::Rect bounds,
    TexturedBatch &batch
):
    batch(batch),
    name(name),
    bounds(bounds)
{
}

void Batcher::BSprite::addCentered(glm::mat3 transform) const {
    // TODO: this. just need to figure out verts based on sprite size then
    //       transform with the matrix.
}

void Batcher::BSprite::add(Util::Rect dst) const {
    if (batch.n >= batch.max) {
        fprintf(stderr, "Texture batch max items reached\n");
        return;
    }
    float z = *batch.total++;
    batch.setVert(batch.n * 4, dst.pos, z, bounds.pos);
    batch.setVert(batch.n * 4 + 1, dst.r(), z, bounds.r());
    batch.setVert(batch.n * 4 + 2, dst.tr(), z, bounds.tr());
    batch.setVert(batch.n * 4 + 3, dst.tl(), z, bounds.tl());
    batch.n++;
}

Batcher::Batcher(int maxSprites, char const *filename) {
    TokenReader reader(filename);
    nTextures = reader.nextInt();
    sprites = hashmap_new(16, 0, 0, 0, spriteHash, spriteCompare, NULL, NULL);
    batches = new TexturedBatch[nTextures];
    int i = 0;
    while (reader.hasMore()) {
        fprintf(stderr, "->\n");
        char const *type = reader.next();
        if (type[0] == 'p') {
            char const *file = reader.next();
            batches[i++] = TexturedBatch(
                new Texture(file),
                maxSprites,
                &nFrame
            );
        } else {
            char const *name = reader.next();
            fprintf(stderr, "%s\n", name);
            BSprite sprite(
                strdup(name),
                Rect(reader.nextInt(), reader.nextInt(), reader.nextInt(), reader.nextInt()),
                batches[i]
            );
            hashmap_set(sprites, &sprite);
        }
    }
    fprintf(stderr, "DAS ENDE\n");
}

Batcher::BSprite const *Batcher::getSprite(char const *name) const {
    return (Batcher::BSprite *)hashmap_get(sprites, name);
}

void Batcher::render() {
    for (int i = 0; i < nTextures; i++) batches[i].render();
}

void Batcher::clear() {
    for (int i = 0; i < nTextures; i++) batches[i].clear();
}