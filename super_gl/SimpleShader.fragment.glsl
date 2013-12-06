#version 400

 in  vec4 vertex_normal;
 out vec4 theNormal;
 out vec4 out_Color;

void main(void)
{
	out_Color = vertex_normal;
	//float diffuse_value = max(dot(vertex_normal, vertex_light_position), 0.0);
	out_Color[0] = vertex_normal[0];
	out_Color[1] = 0.5;
	out_Color[2] = 0.5;
	out_Color[3] = 1;
	out_Color = vertex_normal;
}