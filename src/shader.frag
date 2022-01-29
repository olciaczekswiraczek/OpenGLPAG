#version 330 core

in vec2 textureCoord;
out vec4 color;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture1;

uniform vec4 uniformColor;

void main()
{
	color = uniformColor;//*mix( texture(ourTexture, textureCoord), texture(ourTexture1, textureCoord), 0.1 );
	
}