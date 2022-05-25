#include "common.h"

#include "engine/shader.h"
#include "engine/texture.h"
#include "engine/engine.h"
#include "engine/window.h"
#include "engine/renderer.h"
#include "engine/camera.h"
#include "engine/input.h"


int main(int argc, char* argv[]) {

    // Initialize the engine
    if (!EMBER_InitEngine(argc, argv)) {
        return -1;
    }

    // Window handle
    GLFWwindow* window = EMBER_GLFWwindow();

    // Window size
    vec2s win_size = EMBER_GetWindowSize();

    // Load texture
    String path = EMBER_GetRelativePath("res/texture/bunny.png");
    EMBER_Texture* texture = EMBER_TextureNew(path.buffer, GL_NEAREST);
    STRING_FREE(&path);

    // Load font
    String font_path = EMBER_GetRelativePath("res/font/FiraMono-Regular.ttf");
    EMBER_Font* font = EMBER_FontNew(font_path.buffer, 12, GL_NEAREST);
    STRING_FREE(&font_path);

    // Create camera
    EMBER_Camera* camera = EMBER_CameraNew(0, 1280, 0, 720);

    // Get default shaders
    const EMBER_Shader* quad_shader = EMBER_GetDefaultQuadShader();
    const EMBER_Shader* text_shader = EMBER_GetDefaultTextShader();

    // Fps timer
    double fps_timer = 3.0;
    char fps_buffer[32] = "FPS: 0000";
    vec2s fps_size = EMBER_FontGetTextSize(font, fps_buffer, 1.0f);

    while(!EMBER_WindowShouldClose()) {

        // Pre-loop
        EMBER_CalculateFrametime();

        uint32_t fps = EMBER_GetFPS();
        double delta = EMBER_GetDeltaTime();

        // Print FPS
        fps_timer += delta;
        if (fps_timer >= 0.5) {
            snprintf(fps_buffer, 32, "FPS: %d", fps);
            fps_timer = 0;
        }

        // Events
        LIST_TYPE(EMBER_Event) events = EMBER_GetEvents();

        for (int32_t i =0; i < events->count; ++i) {
            EMBER_Event e = LIST_GET(events, i);

            switch (e.type) {
            case EMBER_EVENT_KEY_PRESS:
                if (e.key == GLFW_KEY_ESCAPE && e.action == EMBER_RELEASE) {
                    EMBER_SetWindowShouldClose(true);
                }
                break;
            default:
                break;
            }
        }

        vec2s cursor_pos = EMBER_GetCursorPos();

        // Update

        // Render
        EMBER_ClearColor(0.06f, 0.01f, 0.12f);
        EMBER_Clear(GL_COLOR_BUFFER_BIT);

        EMBER_ShaderBind(quad_shader);
        EMBER_CameraBegin(camera);

        for (int32_t i = 0; i < 1000; ++i) {
            EMBER_RenderTexture(
                texture,
                NULL,
                (vec3){50.0f, 50.0f, -1.0f},
                (vec2){75.0f, 75.0f},
                (vec4){1.0f, 1.0f, 1.0f, 1.0f}
            );
        }

        EMBER_RenderQuad(
            (vec3) {cursor_pos.x, cursor_pos.y, -1.0},
            (vec2) {50.0, 50.0},
            (vec4) {0.0, 1.0, 0.0, 1.0}
        );

        EMBER_CameraEnd();
        EMBER_ShaderUnbind();

        // Render the text
        EMBER_ShaderBind(text_shader);

        EMBER_RenderText(
            font,
            fps_buffer,
            (vec3) {5, win_size.y - fps_size.y - 5, -1},
            (vec4) {1.0f, 1.0f, 1.0f, 1.0f},
            1.0f
        );

        EMBER_ShaderUnbind();

        EMBER_SwapBuffers();
        EMBER_PollEvents();
    }

    EMBER_TextureFree(texture);
    EMBER_FontFree(font);
    EMBER_CameraFree(camera);

    EMBER_TerminateEngine();

    return 0;
}
