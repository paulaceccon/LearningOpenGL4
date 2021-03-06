// Include OpenGl
#include <glm/glm.hpp>

// Include the standard C++ headers  
#include <vector>
#include <fstream>

// Include custom C++ headers
#include "../Common/ProgramManager.h"


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
	*  Obtain the maximum dimension of the volume, for scaling purposes.
	* 
	*  \return : The maximum dimension (axis value) of the volume.
	**/
	const int MaxDimension() const;
	
	/**
	*  Set the slice to be showed.
	*
	*  \param sliceX : Slice in x.
	*  \param sliceY : Slice in z.
	*  \param sliceY : Slice in z.
	**/
	void TextureSlicing(int sliceX, int sliceY, int sliceZ);
	
	/**
	*  Get the volume's width.
	* 
	*  \return : The volume width.
	**/
	const unsigned int GetWidth() const;
	
	/**
	*  Get the volume's height.
	* 
	*  \return : The volume height.
	**/
	const unsigned int GetHeight() const;
	
	/**
	*  Get the volume's depth.
	* 
	*  \return : The volume depth.
	**/
	const unsigned int GetDepth() const;

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
	
	/**
	*  Upload and bind a texture from file.
	*
	*  \param path : Path to the file to be read.
	*
	*  \return : The texture ID, if everything works fine. 0, otherwise.
	**/
	GLuint UploadTextureFromFile(std::string);
	
	// Volume dimensions.
	unsigned int _width, _height, _depth;
	
	// Texture slicing
	float _currentX, _currentY, _currentZ;

	// Vertex array object, vertex buffer object, index buffer object.
	GLuint _vao, _vbo[2], _ibo, _texture;

	// The OpenGL Matrices.
	glm::mat4 _modelView;
	glm::mat4 _projection;

	// Vector containing the model coordinates, indices and colors.
	std::vector<float> _modelCoordinates, _textureCoordinates;
	std::vector<int> _modelIndices;

	// Program status.
	bool _succeed;
};

