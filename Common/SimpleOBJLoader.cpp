// Include the standard C++ headers  
#include <map>
#include <fstream>

// Include custom C++ headers
#include "SimpleOBJLoader.h"


SimpleOBJLoader::SimpleOBJLoader()
{
}


SimpleOBJLoader::~SimpleOBJLoader()
{
}


bool SimpleOBJLoader::ReadOBJ(const char* path, std::vector<float> &vertices, std::vector<float> &normals, std::vector<unsigned int> &triangulation)
{
	std::map<unsigned int, unsigned int> vertexNormal;
	
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec3> temp_normals;

	FILE * file = fopen(path, "r");
	if (file == NULL)
	{
		printf("Unable to read file!\n");
		return false;
	}

	while (true)
	{
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		else if (strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			unsigned int vertexIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
			if (matches != 6)
			{
				printf("File can't be read");
				return false;
			}
			
			// OBJ starts at 1 
			triangulation.push_back(vertexIndex[0]-1);
			triangulation.push_back(vertexIndex[1]-1);
			triangulation.push_back(vertexIndex[2]-1);

			vertexNormal[vertexIndex[0]-1] = normalIndex[0]-1;
			vertexNormal[vertexIndex[1]-1] = normalIndex[1]-1;
			vertexNormal[vertexIndex[2]-1] = normalIndex[2]-1;
		}
	}
	// Conversion to float vector
	for (unsigned int i = 0; i < temp_vertices.size(); i++)
	{
		glm::vec3 vertex = temp_vertices[i];
		vertices.push_back(vertex.x);
		vertices.push_back(vertex.y);
		vertices.push_back(vertex.z);

		glm::vec3 normal = temp_normals[vertexNormal[i]];
		normals.push_back(normal.x);
		normals.push_back(normal.y);
		normals.push_back(normal.z);
	}
	return true;
}

/*bool SimpleOBJLoader::ReadOBJ(const char* path, std::vector<float> &vertices, std::vector<float> &normals)
{
	std::vector<unsigned int> vertexIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec3> temp_normals;

	FILE * file = fopen(path, "r");
	if (file == NULL)
	{
		printf("Unable to read file!\n");
		return false;
	}

	while (true)
	{
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		else if (strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			unsigned int vertexIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
			if (matches != 6)
			{
				printf("File can't be read");
				return false;
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	// Conversion to float vector
	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		// OBJ starts at 1
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		vertices.push_back(vertex.x);
		vertices.push_back(vertex.y);
		vertices.push_back(vertex.z);
	}

	for (unsigned int i = 0; i < normalIndices.size(); i++)
	{
		unsigned int normalIndex = normalIndices[i];
		// OBJ starts at 1
		glm::vec3 normal = temp_normals[normalIndex - 1];
		normals.push_back(normal.x);
		normals.push_back(normal.y);
		normals.push_back(normal.z);
	}
	return true;
}*/


bool SimpleOBJLoader::ReadOFF(const char* path, std::vector<float> &vertices, std::vector<float> &normals, std::vector<unsigned int> &triangulation)
{
	std::ifstream fileMesh(path);
	if (fileMesh.fail())
	{
		printf("Falha ao abrir o arquivo %s\n", path);
		return false;
	}

	int numberVertexs, numberTriangles;
	fileMesh >> numberVertexs >> numberTriangles;

	vertices.reserve(3 * numberVertexs);
	normals.resize(3 * numberVertexs, 0);

	triangulation.reserve(3 * numberTriangles);

	int lixo;
	float x, y, z;
	//le vertices
	for (int i = 0; i < numberVertexs; i++)
	{
		fileMesh >> x >> y >> z;
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
	}

	int v0,v1, v2, v3;
	//le malha de triangulos
	for (int i = 0; i < numberTriangles; i++)
	{
		fileMesh >> lixo >> v1 >> v2 >> v3;
		triangulation.push_back(v1);
		triangulation.push_back(v2);
		triangulation.push_back(v3);
	}

	for (int i = 0; i < numberTriangles; i++)
	{
		v0 = triangulation[3 * i + 0];
		v1 = triangulation[3 * i + 1];
		v2 = triangulation[3 * i + 2];

		//calcula os vetores
		double x1, y1, z1, x2, y2, z2, x, y, z;
		x1 = vertices[3 * v1 + 0] - vertices[3 * v0 + 0];
		y1 = vertices[3 * v1 + 1] - vertices[3 * v0 + 1];
		z1 = vertices[3 * v1 + 2] - vertices[3 * v0 + 2];

		x2 = vertices[3 * v2 + 0] - vertices[3 * v0 + 0];
		y2 = vertices[3 * v2 + 1] - vertices[3 * v0 + 1];
		z2 = vertices[3 * v2 + 2] - vertices[3 * v0 + 2];

		x = y1 * z2 - z1 * y2;
		y = z1 * x2 - x1 * z2;
		z = x1 * y2 - y1 * x2;

		normals[3 * v0 + 0] += x;
		normals[3 * v0 + 1] += y;
		normals[3 * v0 + 2] += z;

		normals[3 * v1 + 0] += x;
		normals[3 * v1 + 1] += y;
		normals[3 * v1 + 2] += z;

		normals[3 * v2 + 0] += x;
		normals[3 * v2 + 1] += y;
		normals[3 * v2 + 2] += z;
	}
	
	return true;
}
