#pragma once

#include "common.h"

#include "shader.h"
#include "texture.h"


// Renderer Shaders

/* Compiles the default shaders of the renderer, calling this function
   more than once will result in memory leak in GPU and CPU */
bool EMBER_RendererCompileDefaultShaders();


// Renderer Textures

/* Creates the default textures for the renderer, calling this function 
   more than once will result in memory leak in GPU */
bool EMBER_RendererCreateDefaultTextures();


// Initialization

/* Initializes the renderer, compiles the default shaders and creates the default textures. */
bool EMBER_InitRenderer();

void EMBER_TerminateRenderer();


// Batch Management

/* Begins a new batch with a given shader, binding/unbinding a shader during
   a batch is undefined behaviour. */
void EMBER_BeginBatch(const EMBER_Shader* shader);

/* End the current batch and renders the batch */
void EMBER_EndBatch();


// Premade Buffer Templates

/* This function is used for setting vertex data for a quad. Should not be called individually.
   Instead just call 'EMBER_RenderQuad'. */
void _EMBER_SetBatchBufferQuad(vec3s position, vec2s size, vec4s color, vec4s source, uint32_t index);


// Primitive Render Functions

/* Renders a quad with given parameters */
void EMBER_RenderQuad(vec3s position, vec2s size, vec4s color);


// Render Functions

/* Renders a texture with a given parameters */
void EMBER_RenderTexture(const EMBER_Texture* texture, vec3s position, vec2s size, vec4s color);