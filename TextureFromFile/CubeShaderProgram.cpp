#include "CubeShaderProgram.h"

#include <glm/gtc/type_ptr.hpp> 


CubeShaderProgram::CubeShaderProgram()
{
	_vao = 0;
	_vbo[0] = 0;
	_vbo[1] = 0;
	_ibo = 0;
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
  	  	int width, height, depth;
  	  	file >> width;
  	  	file >> height;
  	  	file >> depth;
  	  	
  	  	printf("Texture data: %d, %d, %d\n", width, height, depth);
  	  	
  	  	// Creating buffer
  	  	unsigned char buffer[height][width][depth];
  	  
  	  	// Reading content
  	  	int v;
    	for (int i = 0; i < height; i ++) {
        	for (int j = 0; j < width; j ++) {
            	for (int k = 0; k < depth; k ++) {
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
    	glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, height, width, depth, 0, GL_RED, GL_UNSIGNED_BYTE, buffer);
     
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