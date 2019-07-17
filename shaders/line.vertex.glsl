#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 proj;

void main()
{
	// note that we read the multiplication from right to left
	gl_Position = proj * view  * vec4(aPos, 1.0);
}