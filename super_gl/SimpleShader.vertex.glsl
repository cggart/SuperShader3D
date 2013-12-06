#version 400

//we are, of course, using vec3s but a vec4 is auto cast with the W value as 1.0
//(We need this for the matrix to work)
layout(location=0) in vec4 in_Position;
layout(location=1) in vec4 in_Normal;

out vec4 ex_Color;
out vec4 vertex_normal;
// constants

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(void)
{
	gl_Position = (ProjectionMatrix * ViewMatrix * ModelMatrix) * in_Position;
	vertex_normal = in_Normal;

}