// obj_loader.cpp : Defines the entry point for the console application.
//

#include "wavefront_loader.h"
#include <iostream>

	struct indicesGroup
	{
		int A;
		int B;
		int C;
		int vertIndex;

	};
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
	std::
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
				int vert_1,vert_2,vert_3,normIndexA,normIndexB,normIndexC,temp,uv_indexA,uv_indexB,uv_indexC;
				unsigned int positionIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vert_1, &uv_indexA, &normIndexA, &vert_2, &uv_indexB, &normIndexB, &vert_3, &uv_indexC, &normIndexC );
				//pushing back the vertex
				//Here we need to build re-order everything so that we get an in order vertex stream
				//We get the position of the entire vertex from the position which is our indice
				//However each vertex is defined along with its normal and stuff so we need to "merge" those
				//together to form a solid stream.

				//RE-WRITE THIS GARBAGE YOU NEED TO COMBINE EACH VERT INTO A STRUCT!
				//Objs start at 1 instead of 0, which is nonsense, so we subtract 1

				normal_indices.push_back(normIndexA -1);
				normal_indices.push_back(normIndexB -1);
				normal_indices.push_back(normIndexC -1);

				indices.push_back(vert_1 -1);
				indices.push_back(vert_2 -1);
				indices.push_back(vert_3 -1);


				uv_indices.push_back(uv_indexA -1);
				uv_indices.push_back(uv_indexB -1);
				uv_indices.push_back(uv_indexC -1);

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


	//we merge all three components every time we make a vert
	//so the end result will be 1/3 of the combined uve list
	// (132, 124, ...)  grabing a single value from each vert gives us the total number of verts.




    std::vector<indicesGroup> igroups;
	std::vector<int> finalindices;
		for(int i = 0; i < indices.size(); ++i) 
		{
			indicesGroup igroup;

			//proposed new vertex
			igroup.A = indices[i];
			igroup.B = normal_indices[i];
			igroup.C = uv_indices[i];

			vertex tempVert;
			tempVert.position = posList[indices[i]];
			tempVert.normal = normalList[normal_indices[i]];
			tempVert.uv = uvList[uv_indices[i]];

			//test new vertex to see if its actually new
			int curIndice = i;
			bool notFound = 1;
			if(igroups.size() > 0)
			{
				//check if igroup already exists;
				for(int x=0;x>igroups.size();x++)
				{
					indicesGroup curGroup = igroups[x];
					
					if (curGroup.A == indices[i] &&
						curGroup.B == normal_indices[i] &&
						curGroup.C == uv_indices[i] 
						)
						{
							//we found it already exists
							finalindices.push_back(x);
							//don't add igroup to unique list, its not unique!
							notFound = 0;
							break;
						}
				}
			}
				if(notFound)
				{
					//We didn't find it!
					//igroup must be brand new!
					//lets add it to the group!
					igroup.vertIndex = i;
					
					igroups.push_back(igroup);
					//add the indice ITS NEW!;
					finalindices.push_back(i);
					//add the vert ITS NEW!
					finalList.push_back(tempVert);
					//must be knew just just add it
				}
			//check to see if this vert alread exists, and if it does, grab its index.
			//other wise its new so i is the current indice.
		}

	gModel_3d* theModel = new gModel_3d(finalList,normalList,colorList,uvList,finalindices);
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



