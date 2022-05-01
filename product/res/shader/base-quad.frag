#version 330 core

in vec4 v_color;
in vec2 v_tex_coords;
flat in float v_tex_index;

uniform sampler2D u_textures[16];

out vec4 color;

void main() {
    int index = int(v_tex_index);
    color = texture(u_textures[index], v_tex_coords) * v_color;
}