// Include OpenGL
#include <glm/glm.hpp>

// Include the standard C++ headers  
#include <vector>

// Include custom C++ headers
#include "../Common/ProgramManager.h"


class TesselationShaderProgram : public virtual ProgramManager
{
public:
	/**
	*  Default constructor.
	**/
	TesselationShaderProgram();

	/**
	*  Default destructor.
	**/
	~TesselationShaderProgram();

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
	void CreateShaderProgram(const std::string modelPath);

	/**
	*  Set the tessellation level.
	*
	*  \param tessLevel : the tessellation level.
	**/
	void SetTessellationLevel(float tessLevel);

	/**
	*  Turns on/off the wireframe view mode.
	**/
	void SetWireframeMode(int wireframeMode);

	/**
	*  Change tessellation interpolation mode.
	**/
	void SetInterpolationMode(int interpolationMode);

private:
	/**
	*  Constructs the model, defining its coordinates.
	*
	*  \param modelPath : The model .obj path.
	**/
	void BuildModel(const std::string modelPath);

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

	// Vector containing the model coordinates and normals.
	std::vector<float> _modelCoordinates, _modelNormals;

	// Vector containing the model triangulation.
	std::vector<unsigned int> _modelTriangulation;

	// Program status.
	bool _succeed;

	// Tessellation level.
	float _tessLevel;

	// Wireframe mode.
	int _wireframeOn = 1;

	// Interpolation mode.
	int _interpolationMode = 1;
};

