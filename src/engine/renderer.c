#include "renderer.h"

#include "engine.h"
#include "camera.h"


// Default Textures

static uint32_t s_ember_quad_texture_id_;


// Default Shaders

static const char* s_ember_default_quad_shader_source_[2] = {
    "res/shader/base-quad.vert",
    "res/shader/base-quad.frag"
};
static const char* s_ember_default_text_shader_source_[2] = {
    "res/shader/base-text.vert",
    "res/shader/base-text.frag"
};

static EMBER_Shader* s_ember_default_quad_shader_ = NULL;
static EMBER_Shader* s_ember_default_text_shader_ = NULL;


// Renderer

typedef struct Renderer {
    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;
} Renderer;

static Renderer s_ember_renderer_;


// Renderer Shaders

bool EMBER_RendererCompileDefaultShaders() {

    // Get path for Quad shader
    String quad_shader_vert_path = EMBER_GetRelativePath(
        s_ember_default_quad_shader_source_[0]
    );
    String quad_shader_frag_path = EMBER_GetRelativePath(
        s_ember_default_quad_shader_source_[1]
    );

    // Create the Quad shader
    s_ember_default_quad_shader_ = EMBER_ShaderNew(
        quad_shader_vert_path.buffer,
        quad_shader_frag_path.buffer
    );

    // Get path for Text shader
    String text_shader_vert_path = EMBER_GetRelativePath(
        s_ember_default_text_shader_source_[0]
    );
    String text_shader_frag_path = EMBER_GetRelativePath(
        s_ember_default_text_shader_source_[1]
    );
    
    // Create the Text shader
    s_ember_default_text_shader_ = EMBER_ShaderNew(
        text_shader_vert_path.buffer,
        text_shader_frag_path.buffer
    );

    // Free the memory allocated for strings
    STRING_FREE(&quad_shader_vert_path);
    STRING_FREE(&quad_shader_frag_path);
    STRING_FREE(&text_shader_vert_path);
    STRING_FREE(&text_shader_frag_path);

    // Return the compilation result
    return (s_ember_default_quad_shader_ && s_ember_default_text_shader_);
}

const EMBER_Shader* EMBER_GetDefaultQuadShader() {
    return s_ember_default_quad_shader_;
}

const EMBER_Shader* EMBER_GetDefaultTextShader() {
    return s_ember_default_text_shader_;
}


// Renderer Textures

bool EMBER_RendererCreateDefaultTextures() {

    // Quad texture data
    uint32_t texture_data = 0xFFFFFFFF;

    // Create the Quad texture
    glGenTextures(1, &s_ember_quad_texture_id_);
    glBindTexture(GL_TEXTURE_2D, s_ember_quad_texture_id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}


// Initialization

bool EMBER_InitRenderer() {

    // Compile Shaders
    if (!EMBER_RendererCompileDefaultShaders()) {
        printf("ERROR: Renderer failed to compile default shaders.\n");
        return false;
    }

    // Create Textures
    if (!EMBER_RendererCreateDefaultTextures()) {
        printf("ERROR: Renderer failed to create default textures.\n");
        return false;
    }

    // Create renderer
    s_ember_renderer_ = (Renderer) {
        .vao = 0,
        .vbo = 0,
        .ebo = 0,
    };

    // Set up the vertex data buffer
    float vertex_buffer_data[] = {
            // Position         // Indices
            0.0f, 0.0f, 1.0f,      0.0f,
            1.0f, 0.0f, 1.0f,      1.0f,
            1.0f, 1.0f, 1.0f,      2.0f,
            0.0f, 1.0f, 1.0f,      3.0f
    };

    int32_t vertex_buffer_size = sizeof(vertex_buffer_data) / 4;

    // Set up the index data buffer
    uint32_t index_buffer_data[] = {
            0, 1, 2,
            2, 3, 0
    };

    // Initialize the renderer
    glGenVertexArrays(1, &s_ember_renderer_.vao);
    glBindVertexArray(s_ember_renderer_.vao);

    glGenBuffers(1, &s_ember_renderer_.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_ember_renderer_.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), (const void*) vertex_buffer_data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_buffer_size, (const void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, vertex_buffer_size, (const void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &s_ember_renderer_.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ember_renderer_.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer_data), (const void*) index_buffer_data, GL_STATIC_DRAW);

    // Unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

void EMBER_TerminateRenderer() {

    // Delete default shaders
    EMBER_ShaderFree(s_ember_default_quad_shader_);
    EMBER_ShaderFree(s_ember_default_text_shader_);

    // Delete default texture
    glDeleteTextures(1, &s_ember_quad_texture_id_);

    // Delete renderer
    glDeleteVertexArrays(1, &s_ember_renderer_.vao);

    glDeleteBuffers(1, &s_ember_renderer_.vbo);
    glDeleteBuffers(1, &s_ember_renderer_.ebo);
}


// Primitive Render Functions

void EMBER_RenderQuad(vec3 position, vec2 size, vec4 color) {

    // Get the bound shader
    const EMBER_Shader* bound_shader = EMBER_GetBoundShader();
    if (!bound_shader) {
        bound_shader = s_ember_default_quad_shader_;
        EMBER_ShaderBind(bound_shader);
    }

    // Calculate model
    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, position);
    glm_scale(model, (vec3) {size[0], size[1], 1.0f});

    // Calculate mvp
    mat4 mvp;
    const EMBER_Camera* bound_camera = EMBER_GetBoundCamera();
    if (!bound_camera) {
        glm_mat4_mul((vec4*) EMBER_GetDefaultProjection(), model, mvp);
    } else {
        glm_mat4_mul((vec4*) bound_camera->proj, model, mvp);
        glm_mat4_mul((vec4*) bound_camera->view, mvp, mvp);
    }

    // Send uniforms
    EMBER_ShaderSetInt(bound_shader, "u_texture", 0);
    EMBER_ShaderSetVec4(bound_shader, "u_color", color);
    EMBER_ShaderSetMat4(bound_shader, "u_mvp", mvp);

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s_ember_quad_texture_id_);

    // Bind buffers
    glBindVertexArray(s_ember_renderer_.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ember_renderer_.ebo);

    // Render
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*) 0);

    // Unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // Unbind the shader
    EMBER_ShaderUnbind();
}
