#version 440 core

in vec4 pos;

uniform int time;
uniform vec4 color;
out vec4 fragData;

void main()
{
	float val = pos.y + sin((pos.x +( time/100.0 )) * 7)/3;

	//fragData = vec4(color.x, (abs(pow(val, 2)) * 2), color.z, 1);
	fragData = vec4(color.x, color.y, color.z, (abs(pow(val, 2)) * 2));
}