#version 330 core

in vec3 vertex;
in vec3 colors;
out vec4 outColors;

uniform mat4 TG;

void main()  {
	outColors = vec4(colors, 1.0);
    gl_Position = TG * vec4 (vertex, 1.0);
}
