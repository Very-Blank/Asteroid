#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_texture_coordinate;
uniform mat4 model, view, projection;

out vec2 texture_coordinate;

void main() {
    texture_coordinate = a_texture_coordinate;
    gl_Position = projection * view * model * vec4(a_pos, 1.0f);
}
