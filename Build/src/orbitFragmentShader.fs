#version 330 core
out vec4 fragColor;

uniform vec3 color;

void main()
{
	fragColor = vec4(color.x, color.y, color.z, 1.0f);
}