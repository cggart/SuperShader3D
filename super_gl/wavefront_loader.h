
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <vector>
#include "3D_DataTypes.h"

gModel_3d* load_wavefront_obj(char* filepath);
void generateVertexList(std::string filename);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
void generateVertexList(std::vector<std::string>* file);