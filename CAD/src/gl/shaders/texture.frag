#version 450 core

uniform sampler2D tex;

in vec2 tex_coord;

layout(location = 0) out vec4 FragColor;

void main()
{
	FragColor = texture(tex, tex_coord);
}
