#version 450 core

layout(location = 0) out vec4 FragColor;
layout(pixel_center_integer) in vec4 gl_FragCoord;

float A = 5.0;
float B = 10.0;
float C = 1.0;

uniform mat4 matrix;

void main()
{
	// column-major
//	mat4 D = mat4(vec4(A, 0, 0, 0),
//				  vec4(0, B, 0, 0),
//				  vec4(0, 0, C, 0),
//				  vec4(0, 0, 0, -1));
	
	mat4 D = matrix;

	float x = gl_FragCoord.x/800 - 0.5;
	float y = gl_FragCoord.y/800 - 0.5;

	float a = D[2][2];
	float b = x*D[2][0] + y*D[2][1] + x*D[0][2] + y*D[1][2] + D[3][2] + D[2][3];
	float c = x*(x*D[0][0] + y*D[1][0] + D[3][0] + D[0][3])
			+ y*(x*D[0][1] + y*D[1][1] + D[3][1] + D[1][3])
			+ D[3][3];
	float delta = b*b - 4*a*c;

	if (delta > 0)
		FragColor = vec4(0.9, 0.9, 0.9, 1.0);
	else
		FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	
//	FragColor = vec4(x, y, 0.0, 1.0);
}
