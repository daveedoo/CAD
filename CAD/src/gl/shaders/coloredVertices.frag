#version 450 core

in vec3 theColor;
out vec4 FragColor;


void main()
{
	FragColor = vec4(theColor, 1.f);
}
