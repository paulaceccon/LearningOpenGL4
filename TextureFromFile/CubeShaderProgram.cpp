// Include OpenGL  
#include <glm/gtc/type_ptr.hpp> 

// Include the standard C++ headers  
#include <limits>
#include <algorithm>

// Include custom classes
#include "CubeShaderProgram.h"


CubeShaderProgram::CubeShaderProgram()
{
	_vao = 0;
	_vbo[0] = 0;
	_vbo[1] = 0;
	_ibo = 0;
	
	_width = 0, _height = 0, _depth = 0;
	_currentX = 0.0, _currentY = 0.0, _currentZ = 1.0;
	
	_modelCoordinates.resize(24, 0);
	_modelIndices.resize(36, 0);
	_textureCoordinates.resize(24, 0);

	_modelView  = glm::mat4(1.0);
	_projection = glm::mat4(1.0);
}


CubeShaderProgram::~CubeShaderProgram()
{
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(2, _vbo);
	glDeleteBuffers(1, &_ibo);
}


void CubeShaderProgram::CreateShaderProgram()
{
	_succeed = CreateProgram();
	if (_succeed)
	{
		BuildModel();
		BuildVAO();
	}
}


void CubeShaderProgram::DrawModel(const glm::mat4 &projection, const glm::mat4 &modelView)
{
	_modelView = modelView;
	_projection = projection;

	_succeed = Bind();
	if (_succeed)
	{
		BuildVAO();
		glBindVertexArray(_vao);
		LoadUniformVariables();
		glDrawElements(GL_TRIANGLES, _modelIndices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	UnBind();
}


GLuint CubeShaderProgram::UploadTextureFromFile(std::string path)
{
  	std::ifstream file(path);
  
  	if (file.is_open())
  	{
      	// Reading data dimension
  	  	file >> _width;
  	  	file >> _height;
  	  	file >> _depth;	 	  	
  	  	
  	  	// Creating buffer
  	  	unsigned char buffer[_height][_width][_depth];
  	  
  	  	// Reading content
  	  	int v;
    	for (unsigned int i = 0; i < _height; i ++) {
        	for (unsigned int j = 0; j < _width; j ++) {
            	for (unsigned int k = 0; k < _depth; k ++) {
        			file >> v;
        			buffer[i][j][k] = v * 255;
      			}
    		}
  		}
  		
  		// Close the file
  		file.close();
  		
  		// Create one OpenGL texture
  		GLuint textureID;
    	glGenTextures(1, &textureID);

    	// "Bind" the newly created texture : all future texture functions will modify this texture
    	glBindTexture(GL_TEXTURE_3D, textureID);

    	// Give the image to OpenGL
    	glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, _height, _width, _depth, 0, GL_RED, GL_UNSIGNED_BYTE, buffer);
     
    	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                 
		return textureID;                
  	}
  	else printf("Unable to open file %s\n", path.c_str());
  	return 0;
}

const int CubeShaderProgram::MaxDimension() const
{
	int max = std::numeric_limits<int>::min();
	if ((int)_width  > max)                   
		max = _width;                     
	if ((int)_height > max)                    
		max = _height;
	if ((int)_depth  > max)
		max = _depth;	
	return max;
}

const unsigned int CubeShaderProgram::GetWidth() const
{
	return _width;
}

const unsigned int CubeShaderProgram::GetHeight() const
{
	return _height;
}

const unsigned int CubeShaderProgram::GetDepth() const
{
	return _depth;
}

void CubeShaderProgram::BuildModel()
{
	_modelCoordinates = { -1.0, -1.0,  1.0,
                           1.0, -1.0,  1.0,
                           1.0,  1.0,  1.0,
                          -1.0,  1.0,  1.0,
                          -1.0, -1.0, -1.0,
                           1.0, -1.0, -1.0,
                           1.0,  1.0, -1.0,
                          -1.0,  1.0, -1.0 };

	_modelIndices = { 0, 1, 2,
                      2, 3, 0,
                      1, 5, 6,
                      6, 2, 1,
                      7, 6, 5,
                      5, 4, 7,
                      4, 0, 3,
                      3, 7, 4,
                      4, 5, 1,
                      1, 0, 4,
                      3, 2, 6,
                      6, 7, 3 };
                      
	_textureCoordinates = {  0.0, 0.0, 1.0,  
                   			 1.0, 0.0, 1.0,
                   			 1.0, 1.0, 1.0,  
                   			 0.0, 1.0, 1.0,
                   			 0.0, 0.0, 0.0,
                   			 1.0, 0.0, 0.0,  
                   			 1.0, 1.0, 0.0,
                   			 0.0, 1.0, 0.0 };
}


void CubeShaderProgram::BuildVAO()
{
	// Generate and bind the vertex array object
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// Generate and bind the vertex buffer object
	glGenBuffers(2, _vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, _modelCoordinates.size() * sizeof(float), &_modelCoordinates[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, _textureCoordinates.size() * sizeof(float), &_textureCoordinates[0], GL_STATIC_DRAW);

	// Generate and bind the index buffer object
	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _modelIndices.size() * sizeof(GLuint), &_modelIndices[0], GL_STATIC_DRAW);
	
	// Generate and bind texture
	_texture = UploadTextureFromFile("dummy.txt");
	if (!_texture)
	{
		printf("Unable to load texture\n");
		exit(0);
	}
		
	LoadAttributeVariables();

	glBindVertexArray(0);
}


void CubeShaderProgram::LoadUniformVariables()
{
	glm::mat4 MV = _modelView;
	glm::mat4 P = _projection;
	
	// OpenGL Matrices 
	GLuint ModelView_location = glGetUniformLocation(GetProgramID(), "mvMatrix");
	glUniformMatrix4fv(ModelView_location, 1, GL_FALSE, glm::value_ptr(MV));

	GLuint Projection_location = glGetUniformLocation(GetProgramID(), "pMatrix");
	glUniformMatrix4fv(Projection_location, 1, GL_FALSE, glm::value_ptr(P));
	
	// Cube texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, _texture);
	GLint Texture_location = glGetUniformLocation(GetProgramID(), "tex");
	glUniform1i(Texture_location, 0);
}


void CubeShaderProgram::LoadAttributeVariables()
{
	// Vertex Attributes
	GLuint VertexPosition_location = glGetAttribLocation(GetProgramID(), "vPosition");
	glEnableVertexAttribArray(VertexPosition_location);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glVertexAttribPointer(VertexPosition_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	GLuint TextureCoord_Location = glGetAttribLocation(GetProgramID(), "texCoord");
	glEnableVertexAttribArray(TextureCoord_Location);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
	glVertexAttribPointer(TextureCoord_Location, 3, GL_FLOAT, GL_FALSE, 0, 0);
}