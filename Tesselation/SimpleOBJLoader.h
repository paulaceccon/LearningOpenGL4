#pragma once

// Include the standard C++ headers  
#include <glm/glm.hpp>

// Include custom C++ headers
#include<vector>

class SimpleOBJLoader
{
public:
	/**
	*  Default constructor.
	**/
	SimpleOBJLoader();
	
	/**
	*  Default destructor.
	**/
	~SimpleOBJLoader();

	/**
	*  Default constructor.
	*
	*  \param path : The model .obj path.
	*  \return vertices : A vector containing the model vertices.
	*  \return normals : A vector containing the model normal for each vertex.
	**/
	bool ReadOBJ(const char* path, std::vector<float> &vertices, std::vector<float> &normals, std::vector<unsigned int> &triangulation);
	bool ReadOFF(const char* path, std::vector<float> &vertices, std::vector<float> &normals);
};

