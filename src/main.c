#include "common.h"

#include "engine/engine.h"
#include "engine/window.h"
#include "engine/texture.h"

#include "ember-std/parser/parser.h"


int main(int argc, char* argv[]) {

    if (!EMBER_InitEngine(argc, argv)) {
        return -1;
    }

    GLFWwindow* window = EMBER_GLFWwindow();

    EMBER_Texture* texture = EMBER_TextureNew("res/texture/ascii.png", GL_FILTER);

    while(!EMBER_WindowShouldClose()) {

        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            EMBER_SetWindowShouldClose(true);
        }

        EMBER_ClearColor(0.06, 0.01, 0.12);
        EMBER_Clear(GL_COLOR_BUFFER_BIT);

        EMBER_SwapBuffers();
        glfwPollEvents();
    }

    EMBER_TextureFree(texture);

    glfwTerminate();

    return 0;
}