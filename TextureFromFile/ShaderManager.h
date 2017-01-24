#pragma once

// Include OpenGL
#include <GL/glew.h>  

// Include the standard C++ headers  
#include<string>

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();
	
	GLuint loadShaderFromFile(std::string path, GLenum shaderType);

private:
	void printShaderLog(GLuint shader);
};

