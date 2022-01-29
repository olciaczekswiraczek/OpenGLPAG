#version 330 core

in vec2 textureCoord;
out vec4 color;

uniform sampler2D texture_diffuse1;

uniform vec4 uniformColor;

void main()
{
	color =texture(texture_diffuse1, textureCoord);
	
}