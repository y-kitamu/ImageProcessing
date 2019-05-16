#version 330 core

out vec3 color;

uniform sampler2D myTextureSampler;

void main() {
    color = texture(myTextureSampler, uv).rgb;
}
