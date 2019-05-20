#version 460 core

in vec2 uv;
out vec4 color;

uniform sampler2DRect image;
// uniform sampler2D image;

void main() {
    // color = vec4(uv.x, uv.y, 0.0, 0.0);
    // color = texture(image, gl_FragCoord.xy); //
    color = texture(image, uv); //
}

