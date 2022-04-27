#include "shader.h"

#include "engine.h"


// Static

static const EMBER_Shader* s_ember_bound_shader_ = NULL;


// Bound Shader

const EMBER_Shader* EMBER_GetBoundShader() {
    return s_ember_bound_shader_;
}


// Compilation

uint32_t EMBER_ShaderCompile(uint32_t type, const char* source) {

    if (source == NULL)
		return 0;

    uint32_t shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {

        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*) malloc(length * sizeof(char));
        glGetShaderInfoLog(shader, length, &length, message);
		
		char* shader_type = (type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
		printf("ERROR: '%s' shader failed to compile.\n", shader_type);
		printf("OUTPUT: %s\n", message);
        
        // Cleanup
        glDeleteShader(shader);

        free((void*) source);
        free(message);

        return 0;
    }

    // Cleanup
    free((void*) source);

    return shader;
}


// Constructor & Destructor

EMBER_Shader* EMBER_ShaderNew(const char* vertex_path, const char* fragment_path) {

    // Allocate memory for the shader
    EMBER_Shader* shader = (EMBER_Shader*) malloc(sizeof(EMBER_Shader));

    // Find paths for source files
    String vertex_string   = EMBER_GetRelativePath(vertex_path);
    String fragment_string = EMBER_GetRelativePath(fragment_path);

    // Create the shader
    uint32_t program = glCreateProgram();
    uint32_t vs = EMBER_ShaderCompile(GL_VERTEX_SHADER,   read_file(vertex_string.buffer));
    uint32_t fs = EMBER_ShaderCompile(GL_FRAGMENT_SHADER, read_file(fragment_string.buffer));

	if (!(vs) || !(fs)) 
		return 0;

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {

        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
		printf("WARNING: Shader program failed to link.\n");
        printf("OUTPUT: %s\n", infoLog);

        // Free memory
        free(shader);

        // Free strings
        STRING_FREE(&vertex_string);
        STRING_FREE(&fragment_string);

        // Cleanup
        glDeleteProgram(program);
        glDeleteShader(vs);
        glDeleteShader(fs);

        return NULL;
    }

    glValidateProgram(program);

    // Free strings
    STRING_FREE(&vertex_string);
    STRING_FREE(&fragment_string);

    // Cleanup
    glDeleteShader(vs);
    glDeleteShader(fs);

    return shader;
}

void EMBER_ShaderFree(EMBER_Shader* shader) {
    if (!shader) {
        return;
    }

    glDeleteProgram(shader->id);

    free(shader);
}


// Binding

void EMBER_ShaderBind(const EMBER_Shader* shader) {
    s_ember_bound_shader_ = shader;
    
    glUseProgram(shader->id);
}

void EMBER_ShaderUnbind() {
    s_ember_bound_shader_ = NULL;

    glUseProgram(0);
}