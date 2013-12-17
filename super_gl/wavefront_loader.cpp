// obj_loader.cpp : Defines the entry point for the console application.
//

#include "wavefront_loader.h"
#include <iostream>
gModel_3d* load_wavefront_obj(char* filepath)
{
	std::vector<pos3> posList;
	std::vector<pos3> normalList;
	std::vector<color4> colorList;
	std::vector<int> indices;
	std::vector<int> normal_indices;
	std::vector<int> uv_indices;
	std::vector<vertex> finalList;
	std::vector<pos2> uvList;
	FILE * file = std::fopen(filepath, "r");
	if( file == NULL )
	{
		std::cout << "Impossible to open the file !\n";
		std::cin.get();
		return false;
	}
	else
	{
		while( 1 )
		{
			char lineHeader[8548];
			// read the first word of the line
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF){break;}// EOF = End Of File. Quit the loop.
			else if ( strcmp( lineHeader, "v" ) == 0 )
			{
				pos3 position;
				fscanf(file, "%f %f %f\n", &position.x, &position.y, &position.z );
				posList.push_back(position);
			}
			else if ( strcmp( lineHeader, "vn" ) == 0 )
			{
				pos3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
				normalList.push_back(normal);
				
			}
			else if ( strcmp( lineHeader, "vt" ) == 0 )
			{
				pos2 texture;
				fscanf(file, "%f %f %f\n", &texture.u, &texture.v);
				uvList.push_back(texture);
			}
			else if ( strcmp( lineHeader, "f" ) == 0 )
			{
				int vert_1,vert_2,vert_3,normIndexA,normIndexB,normIndexC,temp,uv_indexA,uv_indexB;
				unsigned int positionIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vert_1, &uv_indexA, &normIndexA, &vert_2, &uv_indexB, &normIndexB, &vert_3, &temp, &normIndexC );
				//pushing back the vertex
				//Here we need to build re-order everything so that we get an in order vertex stream
				//We get the position of the entire vertex from the position which is our indice
				//However each vertex is defined along with its normal and stuff so we need to "merge" those
				//together to form a solid stream.


				//Objs start at 1 instead of 0, which is nonsense, so we subtract 1
				normIndexA = normIndexA  -1;
				normIndexB = normIndexB - 1;
				normIndexC= normIndexC - 1;

				normal_indices.push_back(normIndexA);
				normal_indices.push_back(normIndexB);
				normal_indices.push_back(normIndexC);

				vert_1 = vert_1  -1;
				vert_2 = vert_2 - 1;
				vert_3 = vert_3 - 1;

				indices.push_back(vert_1);
				indices.push_back(vert_2);
				indices.push_back(vert_3);

				uv_indexA = uv_indexA  -1;
				uv_indexB = uv_indexB - 1;

				uv_indices.push_back(uv_indexA);
				uv_indices.push_back(uv_indexB);


				//Here we merge our positions & normals into one array.
				//the normalIndex and positionIndex array should be the same length

					if (matches != 9)
					{
						std::cout << "File can't be read by our simple parser : ( Try exporting with other options\n";
						return false;
					}
			}
		}
	}
	int numberOfVerts = posList.size();
	vertex *final  = new vertex[numberOfVerts];
	int test = sizeof(*final);

	//We divide the total number of indices by 3 since there
	//are an expected 3 elements per vert (pos,uv, & normal).
	//Keep in mind that the total number of vetices will
	//exceed the number of positions as a unique vertice
	//must be created for each unique element (normal, uv, ...)
	//in order to get the true vert count we divide indices
	//instead of looking directly at the verts.

	finalList.resize(indices.size()/3);
	int bo0b = indices.size()/3;
	int tingsize = uv_indices.size()/sizeof(pos2);
	int w = sizeof(pos3);
		for(int i = 0; i < indices.size()/3; ++i) 
		{
			vertex tempVert;
			tempVert.position = posList[indices[i]];
			tempVert.normal = normalList[normal_indices[i]];
			tempVert.uv = uvList[uv_indices[i]];
			finalList[indices[i]] = tempVert;
		}

	gModel_3d* theModel = new gModel_3d(finalList,normalList,colorList,uvList,indices);
	return theModel;
}
std::string get_file_contents(const char *filename)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    return(std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()));
  }
  throw(errno);
}



