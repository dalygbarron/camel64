#include <libdragon.h>
#include <GL/gl.h>
#include <GL/gl_enums.h>
#include <GL/gl_integration.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string.h>
#include "glm/gtc/type_ptr.hpp"
#include "Util.hpp"
#include "Texture.hpp"
#include "Skybox.hpp"
#include "Spatial.hpp"
#include "Batcher.hpp"

#define DEG2RAD(d) ((d) * M_PI/180.)
#define RAD2DEG(r) ((r) / (M_PI/180.))

uint32_t const WHITE = 0xffffffff;

int main() {
    debug_init(DEBUG_FEATURE_ALL);

    assert(dfs_init( DFS_DEFAULT_LOCATION ) == DFS_ESUCCESS);

    // Initialize 320x240x16, triple buffering.
    // NOTE: anti-alias will only be active if activated later in GL.
    display_init(
        RESOLUTION_320x240,
        DEPTH_16_BPP,
        3,
        GAMMA_NONE,
        ANTIALIAS_RESAMPLE_FETCH_ALWAYS
    );
 
    // Allocate a buffer that will be used as Z-Buffer.
    surface_t zbuffer = surface_alloc(FMT_RGBA16, 320, 240);

    // Initialize OpenGL
    gl_init();
    //GLuint cube = generateCube();

    float aspect_ratio = (float)display_get_width() /
        (float)display_get_height();
    float near_plane = 0.5f;
    float far_plane = 120.0f;

    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(
        -near_plane*aspect_ratio,
        near_plane*aspect_ratio,
        -near_plane,
        near_plane,
        near_plane,
        far_plane
    );
    glMatrixMode(GL_MODELVIEW);

    joypad_init();

    dfs_init(DFS_DEFAULT_LOCATION);
    audio_init(44100, 4);
    mixer_init(16);
    mixer_set_vol(1);

    xm64player_t xmPlayer;
    xm64player_open(&xmPlayer, "rom:/music/bigAssClouds.xm64");
    xm64player_play(&xmPlayer, 0);

    Util::initFonts();

    Texture texture("rom:/pics/texture.sprite");
    Texture junk("rom:/pics/wood.sprite");
    //Batcher batch(256, "rom:/sprites.toke");

    //Batcher::BSprite const *sprite = batch.getSprite("cancel.png");

    sprite_t *mario = sprite_load("rom:/pics/logo.sprite");

    Skybox sky("rom:/pics/sky.sprite");

    Spatial camera;

    model64_t *monkey = model64_load("rom:/models/monkey.model64");
    GLuint cube = glGenLists(1);
    glNewList(cube, GL_COMPILE);
    model64_draw(monkey);
    glEndList();


    glm::vec2 polar(M_PI_2);
    float xSin = fm_sinf(polar.x);
    float xCos = fm_cosf(polar.x);
    float ySin = fm_sinf(polar.y);
    float yCos = fm_cosf(polar.y);
    camera.setPosition(glm::vec3(ySin * xCos, yCos, ySin * xSin) * 38.0f);
    camera.setRotation(glm::vec3(-polar.y + M_PI_2, -polar.x + M_PI_2, 0));
    int audiosz = audio_get_buffer_length();
    
    // Main loop
    while (1) {
        while (!audio_can_write()) {}
        int16_t *out = audio_write_begin();
        mixer_poll(out, audiosz);
        audio_write_end();


        joypad_poll();
        joypad_inputs_t inputs = joypad_get_inputs(JOYPAD_PORT_1);
        polar += glm::vec2(-inputs.stick_x, inputs.stick_y) * 0.0019f;
        xSin = fm_sinf(polar.x);
        xCos = fm_cosf(polar.x);
        ySin = fm_sinf(polar.y);
        yCos = -fm_cosf(polar.y);
        camera.setPosition(glm::vec3(ySin * xCos, yCos, ySin * xSin) * 38.0f);
        camera.setRotation(glm::vec3(-polar.y + M_PI_2, -polar.x + M_PI_2, 0));

        // Acquire a framebuffer. Wait until it's available.
        surface_t *fb = display_get();

        // Attach RDP to the framebuffer and the Z-Buffer.
        // From now on, the RDP will draw to the specified buffer(s).
        rdpq_attach(fb, &zbuffer);

        // Enter the OpenGL context. From now on, you can start using OpenGL
        // and you must NOT use rdpq to avoid conflicts.
        gl_context_begin();

        
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_NORMALIZE);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glLoadIdentity();

        //sprite->add(Util::Rect(0, 0, 100, 200));



        {
            glm::vec3 rot = camera.getRotation();
            glRotatef(RAD2DEG(-rot.x), 1, 0, 0);
            glRotatef(RAD2DEG(-rot.y), 0, 1, 0);
            glRotatef(RAD2DEG(-rot.z), 0, 0, 1);
            sky.render(glm::vec3(0, 0, 1));

            glm::mat4 cameraMatrix = glm::inverse(camera.getMatrix());
            glLoadMatrixf(glm::value_ptr(cameraMatrix));

            glEnable(GL_CULL_FACE);
            glClear(GL_DEPTH_BUFFER_BIT);
            
            junk.bind();

            float timer = (float)get_ticks() / TICKS_PER_SECOND * 19;
            float xTwist = fm_sinf(timer * 0.1f);
            float yTwist = tan(timer * 0.01f);
            float zTwist = fm_cosf(timer * 0.4f);

            glRotatef(xTwist * 30, 1, 0, 0);
            glRotatef(yTwist * 30, 0, 1, 0);
            glRotatef(zTwist * 30, 0, 0, 1);
            glCallList(cube);
            glTranslatef(0, 5, 0);
            for (int i = 0; i < 2; i++) {
                glPushMatrix();
                glScalef(0.7, 0.7, 0.7);
                glTranslatef(0, 10, 0);
                glRotatef(xTwist * 30, 1, 0, 0);
                glRotatef(yTwist * 30, 0, 1, 0);
                glRotatef(zTwist * 30, 0, 0, 1);
                glCallList(cube);
                glTranslatef(0, 5, 0);
            }

            for (int i = 0; i < 2; i++) glPopMatrix();
        }
        
        gl_context_end();
        
        rdpq_text_printf(
            NULL,
            Util::HEADING,
            20,
            20,
            "HELLO\nIt is Worm Time"
        );
        rdpq_set_mode_standard();
        rdpq_mode_alphacompare(128);
        rdpq_sprite_blit(mario, 200, 0, NULL);
        rdpq_detach_show();
    }
    return 0;
}
