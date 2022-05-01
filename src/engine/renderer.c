#include "renderer.h"

#include "engine.h"


// Renderer Properties

#define S_EMBER_MAX_QUAD_       6000
#define S_EMBER_MAX_INDEX_      6000 * 6
#define S_EMBER_MAX_VERTEX_     6000 * 4
#define S_EMBER_MAX_TEXTURE_    16


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


// Vertex

typedef struct Vertex {
    vec3s position;
    vec4s color;
    vec2s tex_coords;
    float tex_index;
} Vertex;


// Renderer

typedef struct Renderer {
    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;

    uint32_t index_counter;
    uint32_t texture_counter;

    uint32_t bound_textures[S_EMBER_MAX_TEXTURE_];
} Renderer;

static Renderer s_ember_renderer_;


// Batch Data

static Vertex* s_ember_batch_vertex_data_;
static Vertex* s_ember_batch_vertex_data_ptr_;


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

        .index_counter = 0,
        .texture_counter = 0,
    };

    // Setup the vertex data buffer
    size_t vertex_buffer_size = sizeof(Vertex) * S_EMBER_MAX_VERTEX_;
    s_ember_batch_vertex_data_ = (Vertex*) malloc(vertex_buffer_size);

    // Setup the index data buffer
    size_t index_buffer_size = sizeof(uint32_t) * S_EMBER_MAX_INDEX_;
    uint32_t* s_ember_batch_index_data_ = (uint32_t*) malloc(index_buffer_size);

    uint32_t offset = 0;

    for (int32_t i = 0; i < S_EMBER_MAX_INDEX_; i += 6) {

        s_ember_batch_index_data_[i + 0] = 0 + offset;
        s_ember_batch_index_data_[i + 1] = 1 + offset;
        s_ember_batch_index_data_[i + 2] = 2 + offset;

        s_ember_batch_index_data_[i + 3] = 2 + offset;
        s_ember_batch_index_data_[i + 4] = 3 + offset;
        s_ember_batch_index_data_[i + 5] = 0 + offset;

        offset += 4;
    }

    // Initialize the renderer
    glGenVertexArrays(1, &s_ember_renderer_.vao);
    glBindVertexArray(s_ember_renderer_.vao);

    glGenBuffers(1, &s_ember_renderer_.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_ember_renderer_.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, color));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, tex_coords));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offsetof(Vertex, tex_index));

    glGenBuffers(1, &s_ember_renderer_.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ember_renderer_.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_size, s_ember_batch_index_data_, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Free the index data
    free(s_ember_batch_index_data_);

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

    // Delete the vertex data buffer
    free(s_ember_batch_vertex_data_);
}


// Batch Management

void EMBER_BeginBatch(const EMBER_Shader* shader) {
    
    // If shader is not present use the default
    if (!shader) {
        shader = s_ember_default_quad_shader_;
    }
    
    // Bind shader
    EMBER_ShaderBind(shader);

    // Send texture uniforms
    int32_t loc = glGetUniformLocation(shader->id, "u_textures");

    int32_t samplers[S_EMBER_MAX_TEXTURE_];
    for (int32_t i = 0; i < S_EMBER_MAX_TEXTURE_; ++i) {
        samplers[i] = i;
    }

    glUniform1iv(loc, S_EMBER_MAX_TEXTURE_, samplers);

    // Move the vertex data ptr to start
    s_ember_batch_vertex_data_ptr_ = s_ember_batch_vertex_data_;

    // Reset renderer
    s_ember_renderer_.index_counter = 0;
    s_ember_renderer_.texture_counter = 1;

    for (int32_t i = 0; i < S_EMBER_MAX_TEXTURE_; ++i) {
        s_ember_renderer_.bound_textures[i] = 0;
    }

    // Emplace default texture to bound ones
    s_ember_renderer_.bound_textures[0] = s_ember_quad_texture_id_;
}

void EMBER_EndBatch() {

    // Upload data to renderer
    glBindBuffer(GL_ARRAY_BUFFER, s_ember_renderer_.vbo);
    glBufferSubData(
        GL_ARRAY_BUFFER, 
        0,
        (s_ember_renderer_.index_counter / 6) * (4 * sizeof(Vertex)),
        s_ember_batch_vertex_data_
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Bind and active textures
    for (int32_t i = 0; i < S_EMBER_MAX_TEXTURE_; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, s_ember_renderer_.bound_textures[i]);
    }

    // Render the batch
    glBindVertexArray(s_ember_renderer_.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_ember_renderer_.ebo);

    glDrawElements(
        GL_TRIANGLES, 
        (int32_t) s_ember_renderer_.index_counter, 
        GL_UNSIGNED_INT, 
        NULL
    );

    // Unbind the buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Unbind the shader
    EMBER_ShaderUnbind();
}


// Premade Buffer Templates

void _EMBER_SetBatchBufferQuad(vec3s position, vec2s size, vec4s color, vec4s source, uint32_t index) {

    // Set vertices
    s_ember_batch_vertex_data_ptr_->position = (vec3s) {position.x, position.y, position.z};
    s_ember_batch_vertex_data_ptr_->color = color;
    s_ember_batch_vertex_data_ptr_->tex_coords = (vec2s) {source.x, source.y};
    s_ember_batch_vertex_data_ptr_->tex_index = index;
    s_ember_batch_vertex_data_ptr_++;

    s_ember_batch_vertex_data_ptr_->position = (vec3s) {position.x + size.x, position.y, position.z};
    s_ember_batch_vertex_data_ptr_->color = color;
    s_ember_batch_vertex_data_ptr_->tex_coords = (vec2s) {source.x + source.z, source.y};
    s_ember_batch_vertex_data_ptr_->tex_index = index;
    s_ember_batch_vertex_data_ptr_++;

    s_ember_batch_vertex_data_ptr_->position = (vec3s) {position.x + size.x, position.y + size.y, position.z};
    s_ember_batch_vertex_data_ptr_->color = color;
    s_ember_batch_vertex_data_ptr_->tex_coords = (vec2s) {source.x + source.z, source.y + source.w};
    s_ember_batch_vertex_data_ptr_->tex_index = index;
    s_ember_batch_vertex_data_ptr_++;

    s_ember_batch_vertex_data_ptr_->position = (vec3s) {position.x, position.y + size.y, position.z};
    s_ember_batch_vertex_data_ptr_->color = color;
    s_ember_batch_vertex_data_ptr_->tex_coords = (vec2s) {source.x, source.y + source.w};
    s_ember_batch_vertex_data_ptr_->tex_index = index;
    s_ember_batch_vertex_data_ptr_++;

    // Increase the index
    s_ember_renderer_.index_counter += 6;
}


// Primitive Render Functions

void EMBER_RenderQuad(vec3s position, vec2s size, vec4s color) {

    // Reset the batch after max index or max texture is reached    
    if ((s_ember_renderer_.index_counter >= S_EMBER_MAX_INDEX_) ||
        (s_ember_renderer_.texture_counter >= S_EMBER_MAX_TEXTURE_)) {

        const EMBER_Shader* bound_shader = EMBER_GetBoundShader();

        EMBER_EndBatch();
        EMBER_BeginBatch(bound_shader);
    }

    // Calculate source rect
    vec4s source = {0, 0, 1, 1};

    // Set vertices
    _EMBER_SetBatchBufferQuad(position, size, color, source, 0);
}


// Render Functions

void EMBER_RenderTexture(const EMBER_Texture* texture, vec3s position, vec2s size, vec4s color) {

    // Reset the batch after max index or max texture is reached    
    if ((s_ember_renderer_.index_counter >= S_EMBER_MAX_INDEX_) ||
        (s_ember_renderer_.texture_counter >= S_EMBER_MAX_TEXTURE_)) {

        const EMBER_Shader* bound_shader = EMBER_GetBoundShader();

        EMBER_EndBatch();
        EMBER_BeginBatch(bound_shader);
    }

    // Set texture and index
    int32_t texture_index = -1;
    for (int32_t i = 0; i < S_EMBER_MAX_TEXTURE_; ++i) {
        uint32_t crnt = s_ember_renderer_.bound_textures[i];

        if (crnt == texture->id) {
            texture_index = i;
        }
    }

    if (texture_index == -1) {
        texture_index = s_ember_renderer_.texture_counter++;
        s_ember_renderer_.bound_textures[texture_index] = texture->id;
    }

    // Calculate source rect
    vec4s source = {0, 0, 1, 1};

    // Set vertices
    _EMBER_SetBatchBufferQuad(position, size, color, source, texture_index);
}