// Include the standard C++ headers  
#include <vector>
#include <fstream>
#include <string>
#include <glm/glm.hpp>

// Include custom C++ headers
#include "ProgramManager.h"


class CubeShaderProgram : public virtual ProgramManager
{
public:
	/**
	*  Default constructor.
	**/
	CubeShaderProgram();

	/**
	*  Default destructor.
	**/
	~CubeShaderProgram();

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
	
	/**
	*  Create the tesselation shader program.
	*
	*  \param path : Path to the file to be read.
	*
	*  \return : The texture ID, if everything works fine. 0, otherwise.
	**/
	GLuint UploadTextureFromFile(std::string);

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
	GLuint _vao, _vbo[2], _ibo, _texture;

	// The OpenGL Matrices.
	glm::mat4 _modelView;
	glm::mat4 _projection;

	// Vector containing the model coordinates, indices and colors.
	std::vector<float> _modelCoordinates, _modelColors, _textureCoordinates;
	std::vector<int> _modelIndices;

	// Program status.
	bool _succeed;
};

