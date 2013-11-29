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
				int vert_1,vert_2,vert_3,temp;
				unsigned int positionIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vert_1, &temp, &temp, &vert_2, &temp, &temp, &vert_3, &temp, &temp );
				//pushing back the vertex
				vert_1 = vert_1  -1;
				vert_2 = vert_2 - 1;
				vert_3 = vert_3 - 1;
				indices.push_back(vert_1);
				indices.push_back(vert_2);
				indices.push_back(vert_3);
					if (matches != 9)
					{
						std::cout << "File can't be read by our simple parser : ( Try exporting with other options\n";
						return false;
					}
			}
		}
	}

	gModel_3d* theModel = new gModel_3d(posList,normalList,colorList,uvList,indices);
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



