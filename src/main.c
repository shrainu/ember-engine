#include "common.h"

#include "engine/shader.h"
#include "engine/texture.h"
#include "engine/engine.h"
#include "engine/window.h"
#include "engine/renderer.h"

#include "ember-std/parser/parser.h"


int main(int argc, char* argv[]) {

    if (!EMBER_InitEngine(argc, argv)) {
        return -1;
    }

    GLFWwindow* window = EMBER_GLFWwindow();

    String path = EMBER_GetRelativePath("res/texture/bunny.png");
    EMBER_Texture* texture = EMBER_TextureNew(path.buffer, GL_NEAREST);
    STRING_FREE(&path);

    double fps_timer = 3.0;

    while(!EMBER_WindowShouldClose()) {

        // Pre-loop
        EMBER_CalculateFrametime();

        uint32_t fps = EMBER_GetFPS();
        double delta = EMBER_GetDeltaTime();

        fps_timer += delta;
        if (fps_timer >= 0.5) {
            printf("FPS: %d | Frame Time: %f\n", fps, delta);
            fps_timer = 0;
        }

        // Events
        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            EMBER_SetWindowShouldClose(true);
        }

        // Update

        // Render
        EMBER_ClearColor(0.06, 0.01, 0.12);
        EMBER_Clear(GL_COLOR_BUFFER_BIT);

        // Initiating multiple batches to test the performance
        // On MacOS, initiating 100 batches result in 30 FPS.
        // Which is really low.

        for (int32_t i = 0; i < 100; ++i) {
            EMBER_BeginBatch(NULL);

            EMBER_RenderQuad(
                (vec3s) {0.0, -0.5, -1.0},
                (vec2s) {0.5, 1.0},
                (vec4s) {0.5, 0.12, 1.0, 1.0}
            );

            /*
            EMBER_RenderTexture(
                texture,
                (vec3s) {-0.5, -0.5, -1.0},
                (vec2s) {0.5, 1.0},
                (vec4s) {0.7, 1.0, 0.2, 1.0}
            );
            */

            EMBER_EndBatch();
        }

        EMBER_SwapBuffers();
        glfwPollEvents();
    }

    EMBER_TextureFree(texture);

    EMBER_TerminateEngine();

    return 0;
}