#ifndef _GIZMOSSHADERPROGRAM_H_
#define _GIZMOSSHADERPROGRAM_H_

// Include the standard C++ headers  
#include <vector>
#include <fstream>
#include <string>
#include <glm/glm.hpp>

// Include custom C++ headers
#include "../Common/ProgramManager.h"


class GizmosShaderProgram : public virtual ProgramManager
{
public:
	/**
	*  Default constructor.
	**/
	GizmosShaderProgram();

	/**
	*  Default destructor.
	**/
	~GizmosShaderProgram();

	/**
	*  Draw a model.
	*
	*  \param projection : The OpenGL Projection matrix.
	*  \param modelView : The OpenGL ModelView matrix.
	**/
	void DrawModel(const glm::mat4 &projection, const glm::mat4 &modelView);

	/**
	*  Create the cube shader program.
	**/
	void CreateShaderProgram();
	

private:
	/**
	*  Constructs the model, defining its coordinates.
	*
	**/
	void BuildModel();

	/**
	*  Build the VAO.
	**/
	void BuildVAO();

	/**
	*  Load all attribute variables that a shader program need to run.
	**/
	void LoadAttributeVariables();

	/**
	*  Load all uniform variables that a shader program need to run.
	**/
	void LoadUniformVariables();

	// Vertex array object, vertex buffer object, index buffer object.
	GLuint _vao, _vbo[2], _ibo;

	// The OpenGL Matrices.
	glm::mat4 _modelView;
	glm::mat4 _projection;

	// Vector containing the model coordinates, indices and colors.
	std::vector<float> _modelCoordinates, _modelColors;
	std::vector<int> _modelIndices;

	// Program status.
	bool _succeed;
};

#endif