#include "Skybox.hpp"
#include <libdragon.h>
#include "glm/gtc/type_ptr.hpp"

rdpq_texparms_t textureParam {
    .s {
        .repeats = 0
    },
    .t {
        .repeats = 0
    }
};

struct SkyboxVertex {
    glm::vec3 pos;
    glm::vec2 uv;
    uint8_t colour[3];
};

glm::vec2 const UV_PORTIONS(1.0f / 4.0f, 1.0f / 6.0f);

/**
 * You pass in a reference to the vertices and provide the corners of the skybox
 * cube's actual face, as in not subdivided, and then this code subdivides it
 * into 4 faces and puts all 16 of those vertices into the verts array.
 * @param verts is a pointer to an array of SkyboxVertex which should have at
 *        least 16 elements from here.
 * @param tl is the top left of the skybox face.
 * @param tr is the top right of the skybox face.
 * @param bl is the bottom left of the skybox face.
 * @param br is the bottom right of the skybox face.
*/
void faces(
    struct SkyboxVertex *verts,
    glm::vec3 tl,
    glm::vec3 tr,
    glm::vec3 bl,
    glm::vec3 br
) {
    glm::vec3 topMid = (tl + tr) * 0.5f;
    glm::vec3 leftMid = (tl + bl) * 0.5f;
    glm::vec3 mid = (tl + br) * 0.5f;
    glm::vec3 rightMid = (tr + br) * 0.5f;
    glm::vec3 bottomMid = (bl + br) * 0.5f;
    verts[0].pos = tl;
    verts[1].pos = topMid;
    verts[2].pos = mid;
    verts[3].pos = leftMid;
    verts[4].pos = topMid;
    verts[5].pos = tr;
    verts[6].pos = rightMid;
    verts[7].pos = mid;
    verts[8].pos = leftMid;
    verts[9].pos = mid;
    verts[10].pos = bottomMid;
    verts[11].pos = bl;
    verts[12].pos = mid;
    verts[13].pos = rightMid;
    verts[14].pos = br;
    verts[15].pos = bottomMid;
    for (int i = 0; i < 4; i++) {
        verts[i * 4].uv = glm::vec2();
        verts[i * 4 + 1].uv = glm::vec2(1, 0);
        verts[i * 4 + 2].uv = glm::vec2(1, 1);
        verts[i * 4 + 3].uv = glm::vec2(0, 1);
    }
    for (int i = 0; i < 16; i++) {
        verts[i].colour[0] = 0xff;
        verts[i].colour[1] = 0xff;
        verts[i].colour[2] = 0xff;
    }
}

void loadTexture(sprite_t *sprite, GLuint texture, int x, int y) {
    surface_t slice = sprite_get_tile(sprite, x, y);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSurfaceTexImageN64(GL_TEXTURE_2D, 0, &slice, &textureParam);
}

Skybox::~Skybox() {
    free();
}

void Skybox::init(char const *filename) {
    sprite_t *sprite = sprite_load(filename);
    assertf(
        sprite->width == EXPECTED_WIDTH,
        "can't use %s for skybox because width is %d instead of %d",
        filename,
        sprite->width,
        EXPECTED_WIDTH
    );
    assertf(
        sprite->height == EXPECTED_HEIGHT,
        "can't use %s for skybox because height is %d instead of %d",
        filename,
        sprite->height,
        EXPECTED_HEIGHT
    );
    sprite->hslices = 4;
    sprite->vslices = 6;
    glGenTextures(SKY_N, textures);
    loadTexture(sprite, textures[SKY_FRONT_TOP_LEFT], 0, 0);
    loadTexture(sprite, textures[SKY_FRONT_TOP_RIGHT], 1, 0);
    loadTexture(sprite, textures[SKY_FRONT_BOTTOM_LEFT], 0, 1);
    loadTexture(sprite, textures[SKY_FRONT_BOTTOM_RIGHT], 1, 1);
    loadTexture(sprite, textures[SKY_RIGHT_TOP_LEFT], 2, 0);
    loadTexture(sprite, textures[SKY_RIGHT_TOP_RIGHT], 3, 0);
    loadTexture(sprite, textures[SKY_RIGHT_BOTTOM_LEFT], 2, 1);
    loadTexture(sprite, textures[SKY_RIGHT_BOTTOM_RIGHT], 3, 1);
    loadTexture(sprite, textures[SKY_BACK_TOP_LEFT], 0, 2);
    loadTexture(sprite, textures[SKY_BACK_TOP_RIGHT], 1, 2);
    loadTexture(sprite, textures[SKY_BACK_BOTTOM_LEFT], 0, 3);
    loadTexture(sprite, textures[SKY_BACK_BOTTOM_RIGHT], 1, 3);
    loadTexture(sprite, textures[SKY_LEFT_TOP_LEFT], 2, 2);
    loadTexture(sprite, textures[SKY_LEFT_TOP_RIGHT], 3, 2);
    loadTexture(sprite, textures[SKY_LEFT_BOTTOM_LEFT], 2, 3);
    loadTexture(sprite, textures[SKY_LEFT_BOTTOM_RIGHT], 3, 3);
    loadTexture(sprite, textures[SKY_BOTTOM_TOP_LEFT], 2, 4);
    loadTexture(sprite, textures[SKY_BOTTOM_TOP_RIGHT], 3, 4);
    loadTexture(sprite, textures[SKY_BOTTOM_BOTTOM_LEFT], 2, 5);
    loadTexture(sprite, textures[SKY_BOTTOM_BOTTOM_RIGHT], 3, 5);
    loadTexture(sprite, textures[SKY_TOP_TOP_LEFT], 0, 4);
    loadTexture(sprite, textures[SKY_TOP_TOP_RIGHT], 1, 4);
    loadTexture(sprite, textures[SKY_TOP_BOTTOM_LEFT], 0, 5);
    loadTexture(sprite, textures[SKY_TOP_BOTTOM_RIGHT], 1, 5);
    struct SkyboxVertex vertices[SKY_N * 4];
    glm::vec3 btl(-1, 1, -1);
    glm::vec3 btr(1, 1, -1);
    glm::vec3 bbl(-1, -1, -1);
    glm::vec3 bbr(1, -1, -1);
    glm::vec3 ftl(-1, 1, 1);
    glm::vec3 ftr(1, 1, 1);
    glm::vec3 fbl(-1, -1, 1);
    glm::vec3 fbr(1, -1, 1);
    faces(vertices, ftl, ftr, fbl, fbr); // front
    faces(vertices + 16, ftr, btr, fbr, bbr); // right
    faces(vertices + 32, btr, btl, bbr, bbl); //back
    faces(vertices + 48, btl, ftl, bbl, fbl); // left
    faces(vertices + 64, btl, btr, ftl, ftr); // top
    faces(vertices + 80, fbl, fbr, bbl, bbr); // bottom
    uint16_t indices[SKY_N * 4];
    for (int i = 0; i < SKY_N * 4; i++) indices[i] = i;
    drawLists = glGenLists(6);
    for (int i = 0; i < 6; i++) {
        glNewList(drawLists + i, GL_COMPILE);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(struct SkyboxVertex), (void*)&vertices[0].pos);
        glTexCoordPointer(2, GL_FLOAT, sizeof(struct SkyboxVertex), (void*)&vertices[0].uv);
        glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(struct SkyboxVertex), (void*)&vertices[0].colour);
        for (int f = 0; f < 4; f++) {
            glBindTexture(GL_TEXTURE_2D, textures[i * 4 + f]);
            glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, indices + i * 16 + f * 4);
        }
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisable(GL_COLOR_MATERIAL);
        glEndList();
    }
    initialised = true;
}

void Skybox::free() {
    if (!initialised) return;
    glDeleteTextures(SKY_N, textures);
    initialised = false;
}

void Skybox::render(glm::vec3 cameraDirection) const {
    if (!initialised) {
        debugf("Trying to render uninitialised skybox");
        return;
    }
    for (int i = 0; i < 6; i++) {
        glCallList(drawLists + i);
    }
    // I think basically what we do is for every cardinal direction, we have a
    // vector that we can do a dot product with, and if the dot product is
    // positive (or negative idk), then we will call the display list for those
    // four faces.
} 