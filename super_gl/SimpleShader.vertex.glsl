#version 400

//we are, of course, using vec3s but a vec4 is auto cast with the W value as 1.0
//(We need this for the matrix to work)
//http://stackoverflow.com/questions/18935203/shader-position-vec4-or-vec3
layout(location=0) in vec4 in_Position;
layout(location=1) in vec4 in_Normal;
layout(location=2) in vec2 texCoords;

out vec3 texColor;
out vec3 vertex_normal;
out vec4 eyeCoords;
out vec2 UVs;



// constants

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;


mat3 matrix_inverse_transpose;

mat3 Normal_Matrix = mat3(transpose(inverse(ModelMatrix * ViewMatrix)));

struct material
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};
material mymaterial = material(
  vec4(0.2, 0.2, 0.2, 1.0),
  vec4(1.0, 0.8, 0.8, 1.0),
  vec4(1.0, 1.0, 1.0, 1.0),
  5.0
);
 
void main(void)
{
	UVs=texCoords;
	eyeCoords = ViewMatrix * in_Position;
	vertex_normal = normalize(Normal_Matrix * in_Normal.xyz);
	gl_Position = (ProjectionMatrix * ViewMatrix * ModelMatrix) * in_Position;
}