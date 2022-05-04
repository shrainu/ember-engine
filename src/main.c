#include "common.h"

#include "engine/shader.h"
#include "engine/texture.h"
#include "engine/engine.h"
#include "engine/window.h"
#include "engine/renderer.h"
#include "engine/camera.h"


int main(int argc, char* argv[]) {

    if (!EMBER_InitEngine(argc, argv)) {
        return -1;
    }

    GLFWwindow* window = EMBER_GLFWwindow();

    String path = EMBER_GetRelativePath("res/texture/bunny.png");
    EMBER_Texture* texture = EMBER_TextureNew(path.buffer, GL_NEAREST);
    STRING_FREE(&path);

    double fps_timer = 3.0;

    const EMBER_Shader* shader = EMBER_GetDefaultQuadShader();

    EMBER_Camera* camera = EMBER_CameraNew(0, 1280, 0, 720);

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

        EMBER_ShaderBind(shader);
        EMBER_CameraBegin(camera);

        for (int32_t i = 0; i < 1000; ++i) {
            EMBER_RenderTexture(
                    texture,
                    (vec3){50.0f, 50.0f, -1.0f},
                    (vec2){75.0f, 75.0f},
                    (vec4){1.0f, 1.0f, 1.0f, 1.0f}
            );
        }

        EMBER_CameraEnd();
        EMBER_ShaderUnbind();

        EMBER_SwapBuffers();
        glfwPollEvents();
    }

    EMBER_TextureFree(texture);
    EMBER_CameraFree(camera);

    EMBER_TerminateEngine();

    return 0;
}
