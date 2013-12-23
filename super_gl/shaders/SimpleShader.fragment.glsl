#version 330

 in vec2 UVs;
 in  vec3 vertex_normal;
 in  vec4 eyeCoords;
 out vec3 out_Color;
	uniform sampler2D tex;

 vec3 texColor = texture( tex, UVs ).rgb;


void main(void)
{
	vec3 light;
	
	light[0] = 0;
	light[1] = 0;
	light[2] = 2;

	out_Color[0] = 1;
	out_Color[1] = 1;
	out_Color[2] = 1;
	out_Color = texColor;
	
	//In order to calculate the specular
	//we need to find the difference between the reflect vector
	// & the eye vector
	// This disparity tells how bright the specular reflection should be

	vec3 s = normalize(vec3(light - eyeCoords.xyz));
	vec3 v = normalize(vec3(-eyeCoords.xyz));
	vec3 r = reflect (-s, vertex_normal.xyz);
		  float sDotN = max(dot(s,vertex_normal.xyz), 0.0 );

	  float spec = 0;
	  if( sDotN > 0.0 )
	  {
	 spec = pow( max( dot(r,v), 0.0 ), 50);

	}

	float diffuse_value = max(dot(vertex_normal.xyz,normalize(light.xyz)), 0.0) + 0.1 + spec;
	out_Color *=  diffuse_value;
}


