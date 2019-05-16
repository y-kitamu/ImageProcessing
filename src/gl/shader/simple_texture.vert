#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 uv_in;

out vec2 uv;

void main() {
    gl_Position = position;
    uv = uv_in;
}
