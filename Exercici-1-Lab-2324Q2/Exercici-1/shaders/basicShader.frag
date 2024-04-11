#version 330 core

out vec4 FragColor;
in vec4 outColors;

void main() {
    FragColor = outColors;
}