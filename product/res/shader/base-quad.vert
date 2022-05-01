#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec4 a_color;
layout (location = 2) in vec2 a_tex_coords;
layout (location = 3) in float a_tex_index;

out vec4 v_color;
out vec2 v_tex_coords;
flat out float v_tex_index;

void main() {
    gl_Position = vec4(a_pos, 1.0);

    v_color = a_color;
    v_tex_coords = a_tex_coords;
    v_tex_index = a_tex_index;
}

