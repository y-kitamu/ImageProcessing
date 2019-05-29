#version 460 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_color;

out vec3 frag_color;

void main() {
    gl_Position.xyz = in_pos;
    frag_color = in_color;
}
