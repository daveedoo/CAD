#version 450 core

layout(location = 0) in vec3 pos;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;


void main()
{
	gl_Position = projMatrix * viewMatrix * worldMatrix * vec4(pos, 1.0);
}
