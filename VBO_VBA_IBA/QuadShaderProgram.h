// Include OpenGL
#include <glm/glm.hpp>

// Include the standard C++ headers  
#include <vector>

// Include custom C++ headers
#include "../Common/ProgramManager.h"


class QuadShaderProgram : public virtual ProgramManager
{
public:
	/**
	*  Default constructor.
	**/
	QuadShaderProgram();

	/**
	*  Default destructor.
	**/
	~QuadShaderProgram();

	/**
	*  Draw a model.
	*
	*  \param projection : The OpenGL Projection matrix.
	*  \param modelView : The OpenGL ModelView matrix.
	**/
	void DrawModel(const glm::mat4 &projection, const glm::mat4 &modelView);

	/**
	*  Create the tesselation shader program.
	*
	*  \param modelPath : The model .obj path.
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

