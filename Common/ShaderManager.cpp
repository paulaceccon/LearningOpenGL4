// Include the standard C++ headers 
#include<fstream>

// Include custom C++ headers
#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
}


ShaderManager::~ShaderManager()
{
}


GLuint ShaderManager::loadShaderFromFile(std::string path, GLenum shaderType)
{
	// Open file
	GLuint shaderID = 0;
	std::string shaderString;
	std::ifstream sourceFile(path.c_str());

	// Source file loaded
	if (sourceFile)
	{
		// Get shader source
		shaderString.assign((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());

		// Create shader ID
		shaderID = glCreateShader(shaderType);

		// Set shader source
		const GLchar* shaderSource = shaderString.c_str();
		glShaderSource(shaderID, 1, (const GLchar**)&shaderSource, NULL);

		// Compile shader source
		glCompileShader(shaderID);

		// Check shader for errors
		GLint shaderCompiled = GL_FALSE;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
		if (shaderCompiled != GL_TRUE)
		{
			printf("Unable to compile shader %d!\n\nSource:\n%s\n", shaderID, shaderSource);
			printShaderLog(shaderID);
			glDeleteShader(shaderID);
			shaderID = 0;
		}
	}
	else
	{
		printf("Unable to open file %s\n", path.c_str());
	}

	return shaderID;
}


void ShaderManager::printShaderLog(GLuint shader)
{
	// Make sure that we receive a shader
	if (glIsShader(shader))
	{
		// Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		// Get info string length
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// Allocate string
		char* infoLog = new char[maxLength];

		// Get info log
		glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			printf("%s\n", infoLog);
		}

		// Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a shader\n", shader);
	}
}
