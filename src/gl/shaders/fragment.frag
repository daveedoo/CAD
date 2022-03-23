#version 450 core

#define SCR_WIDTH 800
#define SCR_HEIGHT 800

layout(location = 0) out vec4 FragColor;

uniform mat4 matrix;
uniform vec4 color;
uniform uint screen_w;
uniform uint screen_h;

float specular_intensity(vec3 viewer, vec3 normal, float shininess)
{
	return pow(max(dot(viewer, normal), 0.0), shininess);
}

void main()
{
	mat4 D = matrix;

	float x = gl_FragCoord.x/screen_w;
	float y = gl_FragCoord.y/screen_h;

	float a = D[2][2];
	float b = x*(D[2][0] + D[0][2]) + y*(D[2][1] + D[1][2]) + D[3][2] + D[2][3];
	float c = x*(x*D[0][0] + y*D[1][0] + D[3][0] + D[0][3])
			+ y*(x*D[0][1] + y*D[1][1] + D[3][1] + D[1][3])
			+ D[3][3];
	float delta = b*b - 4*a*c;

	if (delta >= 0)
	{
		float z = (-b + sqrt(delta)) / (2*a);

		float df_dx = x*2*D[0][0]
					+ y*(D[1][0] + D[0][1])
					+ z*(D[2][0] + D[0][2])
					+ D[3][0] + D[0][3];
		float df_dy = y*2*D[1][1]
					+ x*(D[1][0] + D[0][1])
					+ z*(D[1][2] + D[2][1])
					+ D[3][1] + D[1][3];
		float df_dz = z*2*D[2][2]
					+ x*(D[2][0] + D[0][2])
					+ y*(D[2][1] + D[1][2])
					+ D[3][2] + D[2][3];
		vec3 gradient = vec3(df_dx, df_dy, df_dz);
		vec3 normal = gradient / length(gradient);

		vec3 viewer = vec3(0, 0, 1);
		float shininess = 1.f;
		float spec = specular_intensity(viewer, normal, shininess);

		FragColor = spec*color;
	}
	else
		discard;
}
