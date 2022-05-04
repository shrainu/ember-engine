#include "common.h"

#include "engine/shader.h"
#include "engine/texture.h"
#include "engine/engine.h"
#include "engine/window.h"
#include "engine/renderer.h"


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
        EMBER_ClearColor(0.06f, 0.01f, 0.12f);
        EMBER_Clear(GL_COLOR_BUFFER_BIT);

        EMBER_ShaderBind(EMBER_GetDefaultQuadShader());

        for (int32_t i = 0; i < 1000; ++i) {
            EMBER_RenderQuad(
                    (vec3){50.0f, 50.0f, -1.0f},
                    (vec2){50.0f, 50.0f},
                    (vec4){0.24f, 0.2f, 0.48f, 1.0f}
            );
        }

        EMBER_ShaderUnbind();

        EMBER_SwapBuffers();
        glfwPollEvents();
    }

    EMBER_TextureFree(texture);

    EMBER_TerminateEngine();

    return 0;
}
