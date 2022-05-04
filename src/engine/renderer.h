#pragma once

#include "common.h"

#include "shader.h"
#include "texture.h"


// Renderer Shaders

/* Compiles the default shaders of the renderer, calling this function
   more than once will result in memory leak in GPU and CPU */
bool EMBER_RendererCompileDefaultShaders();

/* Returns the default quad shader */
const EMBER_Shader* EMBER_GetDefaultQuadShader();

/* Returns the default text shader */
const EMBER_Shader* EMBER_GetDefaultTextShader();


// Renderer Textures

/* Creates the default textures for the renderer, calling this function 
   more than once will result in memory leak in GPU */
bool EMBER_RendererCreateDefaultTextures();


// Initialization

/* Initializes the renderer, compiles the default shaders and creates the default textures. */
bool EMBER_InitRenderer();

void EMBER_TerminateRenderer();


// Primitive Render Functions

/* Renders a quad with given parameters */
void EMBER_RenderQuad(vec3 position, vec2 size, vec4 color);