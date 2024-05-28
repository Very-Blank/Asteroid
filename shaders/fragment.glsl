#version 330 core
out vec4 frag_color;

uniform sampler2D a_texture;

in vec2 texture_coordinate;

void main() {
    vec4 color = texture(a_texture, texture_coordinate);
    if (color.a < 0.1f) {
        discard;
    }
    frag_color = color;
}
