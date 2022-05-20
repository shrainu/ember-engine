#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in float a_index;

uniform mat4 u_mvp;

out vec2 v_tex_coords;

void main() {
    gl_Position = u_mvp * vec4(a_pos, 1.0);
    v_tex_coords = vec2(a_pos.x, 1 - a_pos.y);
}

