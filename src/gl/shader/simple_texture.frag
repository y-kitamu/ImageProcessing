#version 330
// #version 460 core

in vec2 uv;
out vec4 color;

// uniform sampler2DRect image;
uniform sampler2D image;

void main() {
    color = texture2D(image, uv);
}
