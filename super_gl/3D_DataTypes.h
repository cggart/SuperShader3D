#include <vector>

struct pos3
{
	float x;
	float y;
	float z;
};

struct color3
{
	float r;
	float g;
	float b;
};

struct color4
{
	float r;
	float g;
	float b;
	float a;
};

struct pos2
{
	float u;
	float v;
};

struct vertex
{
	pos3 position;
	pos3 normal;
	pos2 uv;
};

	struct mesh
	{
	std::vector<vertex> pos;
	std::vector<pos3> normals;
	std::vector<pos2> uvs;
	std::vector<color4> colors;
	std::vector<int> indices;
	};

class gModel_3d
{
public:

	mesh theMesh;
	gModel_3d::gModel_3d(std::vector<vertex> vertexArg, std::vector<pos3> normalArg, std::vector<color4> colorArg, std::vector<pos2> uvArg, std::vector<int> argIndices)
	{
		theMesh.pos = vertexArg;
		theMesh.normals = normalArg;
		theMesh.colors = colorArg;
		theMesh.uvs = uvArg;
		theMesh.indices = argIndices;
	}
};