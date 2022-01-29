#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 ourColor;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform sampler2D texture_diffuse1;

void main()
{
		ourColor=vec4(vec3(235.0f,200.0f,220.0f),1.0f);
	    gl_Position = projection * view * model * vec4(aPos, 1.0);

}