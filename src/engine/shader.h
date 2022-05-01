#pragma once

#include "common.h"


// Type
 
typedef struct EMBER_Shader {
    uint32_t id;
} EMBER_Shader;


// Short API typedef

#ifdef EMBER_SHORT_API

typedef EMBER_Shader* Shader;

#endif


// Bound Shader

/* Returns the currently bound shader, NULL if no shader is bound. */
const EMBER_Shader* EMBER_GetBoundShader();


// Compilation

/* Compiles a type of shader from given source, returns the compiled shaders id.
   Returns 0 if it failes to compile the shader. */
uint32_t EMBER_ShaderCompile(uint32_t type, const char* source);


// Constructor & Destructor

/* Creates a new shader from given vertex and fragment paths. User has to free the shader afterwards. */
EMBER_Shader* EMBER_ShaderNew(const char* vertex_path, const char* fragment_path);

/* Frees the memory allocated for the shader */
void EMBER_ShaderFree(EMBER_Shader* shader);


// Binding

void EMBER_ShaderBind(const EMBER_Shader* shader);

void EMBER_ShaderUnbind();


// Uniforms

void EMBER_ShaderSetInt(const EMBER_Shader* shader, const char* location, int32_t value);