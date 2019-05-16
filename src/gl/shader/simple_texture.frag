  #version 330 core

out vec4 color;

uniform sampler2DRect image;

void main() {
    // color = vec4(1.0, 1.0, 1.0, 0.0);
    color = texture(image, gl_FragCoord.xy);
}

