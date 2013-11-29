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
	vertex::vertex()
	{
		position = 0;
		normal = 0;
		uv = 0;
	}
	int position;
	int normal;
	int uv;
};

	struct mesh
	{
	std::vector<pos3> pos;
	std::vector<pos3> normals;
	std::vector<pos2> uvs;
	std::vector<color4> colors;
	std::vector<int> indices;
	};

class gModel_3d
{
public:

	mesh theMesh;
	gModel_3d::gModel_3d(std::vector<pos3> posArg, std::vector<pos3> normalArg, std::vector<color4> colorArg, std::vector<pos2> uvArg, std::vector<int> argIndices)
	{
		theMesh.pos = posArg;
		theMesh.normals = normalArg;
		theMesh.colors = colorArg;
		theMesh.uvs = uvArg;
		theMesh.indices = argIndices;
	}
};