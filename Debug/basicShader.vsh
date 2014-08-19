#version 440 core

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

in vec4 position;
out vec4 pos;

void main()
{
	pos = position;
	gl_Position = projection * view * model * position;
}