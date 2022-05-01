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

    // Create the shader
    uint32_t program = glCreateProgram();
    uint32_t vs = EMBER_ShaderCompile(GL_VERTEX_SHADER,   read_file(vertex_path));
    uint32_t fs = EMBER_ShaderCompile(GL_FRAGMENT_SHADER, read_file(fragment_path));

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

        // Cleanup
        glDeleteProgram(program);
        glDeleteShader(vs);
        glDeleteShader(fs);

        return NULL;
    }

    glValidateProgram(program);

    // Cleanup
    glDeleteShader(vs);
    glDeleteShader(fs);

    // Add program to shader
    shader->id = program;

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


// Uniforms

void EMBER_ShaderSetInt(const EMBER_Shader* shader, const char* location, int32_t value) {

    int32_t loc = glGetUniformLocation(shader->id, location);

#ifdef DEBUG
    if (loc == -1) {
        printf("WARNING: '%s' is not a valid uniform location.\n", location);
        return;
    }
#endif

    glUniform1i(loc, value);
}