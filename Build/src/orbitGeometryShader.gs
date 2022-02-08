#version 330 core

#define PI 3.14159265358979324

layout (points) in;
layout (line_strip, max_vertices = 256) out; 

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform int radius;

void createOrbit(vec4 position, mat4 mvp) 
{
	
		for (int j = 0; j <= 360; j = j+2)
		{
			
            gl_Position = mvp * (position + vec4(radius * sin(j/180.0f * PI), radius * cos(j/180.0f * PI), 0.0f, 0.0f));
			EmitVertex();

		}
		EndPrimitive();
}

void main() {
	mat4 mvp = projection * view * model;
    createOrbit(gl_in[0].gl_Position, mvp);
}