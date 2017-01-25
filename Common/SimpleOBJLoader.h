#ifndef _SIMPLEOBJLOADER_H_
#define _SIMPLEOBJLOADER_H_

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
	*  Reads a .obj file.
	*
	*  \param path : The model .obj path.
	*  \return vertices : A vector containing the model vertices.
	*  \return normals : A vector containing the model normal for each vertex.
	*  \return triangulation : A vector containing the model triangulation.
	**/
	bool ReadOBJ(const char* path, std::vector<float> &vertices, std::vector<float> &normals, std::vector<unsigned int> &triangulation);
	
	/**
	*  Reads a .off file.
	*
	*  \param path : The model .off path.
	*  \return vertices : A vector containing the model vertices.
	*  \return normals : A vector containing the model normal for each vertex.
	*  \return triangulation : A vector containing the model triangulation.
	**/
	bool ReadOFF(const char* path, std::vector<float> &vertices, std::vector<float> &normals, std::vector<unsigned int> &triangulation);
};

#endif