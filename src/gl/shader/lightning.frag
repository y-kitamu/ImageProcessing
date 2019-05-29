#version 460 core

in vec3 frag_color;
out vec4 color;

void main() {
    color.xyz = frag_color;
}
