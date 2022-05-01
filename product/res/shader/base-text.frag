#version 330 core

in vec4 v_color;
in vec2 v_tex_coords;

uniform sampler2D u_texture;

out vec4 color;

void main() {
    color = texture(u_texture, v_tex_coords) * v_color;
}