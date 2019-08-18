#version 330

in vec3 frag_color;
out vec4 color;

void main() {
    color.xyz = frag_color;
}
