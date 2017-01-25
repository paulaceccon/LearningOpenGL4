// Include custom headers .
#include "ProgramManager.h"
#include "ShaderManager.h"
#include "GLUtils.h"


ProgramManager::ProgramManager()
{
	_programID = 0;

	_vertexShaderPath = "";
	_fragmentShaderPath = "";
	_tesselationControlPath = "";
	_tesselationEvaluationPath = "";
	_geometryShaderPath = "";

	_vertexShader = 0;
	_fragmentShader = 0;
	_tesselationControlShader = 0;
	_tesselationEvaluationShader = 0;
	_geometryShader = 0;
}


ProgramManager::~ProgramManager()
{
	FreeProgram();
}


GLuint ProgramManager::GetProgramID()
{
	return _programID;
}


void ProgramManager::FreeProgram()
{
	glDeleteProgram(_programID);
}


bool ProgramManager::Bind()
{
	// Use shader
	glUseProgram(_programID);

	// Check for error
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error binding program!\n");
		PrintProgramLog(_programID);
		return false;
	}
	return true;
}


void ProgramManager::UnBind()
{
	// Use default program
	glUseProgram(NULL);
}


void ProgramManager::SetVertexShader(const std::string vertexShaderPath)
{
	_vertexShaderPath = vertexShaderPath;
}


void ProgramManager::SetFragmentShader(const std::string fragmentShaderPath)
{
	_fragmentShaderPath = fragmentShaderPath;
}


void ProgramManager::SetTesselationControlShader(const std::string tesselationControlPath)
{
	_tesselationControlPath = tesselationControlPath;;
}


void ProgramManager::SetTesselationEvaluationShader(const std::string tesselationEvaluationPath)
{
	_tesselationEvaluationPath = tesselationEvaluationPath;
}


void ProgramManager::SetGeometryShader(const std::string geometryShaderPath)
{
	_geometryShaderPath = geometryShaderPath;
}


bool ProgramManager::CreateProgram()
{
	// Generate a program
	_programID = glCreateProgram();

	// To load shaders
	ShaderManager shaderManager;

	// Vertex Shader
	if (_vertexShaderPath.compare("") != 0)
	{
		// Load vertex shader
		_vertexShader = shaderManager.loadShaderFromFile(_vertexShaderPath, GL_VERTEX_SHADER);

		//Check for errors
		if (_vertexShader == 0)
		{
			glDeleteProgram(_programID);
			_programID = 0;
			return false;
		}

		// Attach vertex shader to program
		glAttachShader(_programID, _vertexShader);
	}

	// Tesselation Control Shader
	if (_tesselationControlPath.compare("") != 0)
	{
		// Create tesselation control shader
		_tesselationControlShader = shaderManager.loadShaderFromFile(_tesselationControlPath, GL_TESS_CONTROL_SHADER);

		// Check for errors
		if (_tesselationControlShader == 0)
		{
			glDeleteProgram(_programID);
			_programID = 0;
			return false;
		}

		// Attach tesselation control shader to program
		glAttachShader(_programID, _tesselationControlShader);
	}

	// Tesselation Evaluation Shader
	if (_tesselationEvaluationPath.compare("") != 0)
	{
		// Create tesselation evaluation shader
		_tesselationEvaluationShader = shaderManager.loadShaderFromFile(_tesselationEvaluationPath, GL_TESS_EVALUATION_SHADER);

		// Check for errors
		if (_tesselationEvaluationShader == 0)
		{
			glDeleteProgram(_programID);
			_programID = 0;
			return false;
		}

		// Attach tesselation evaluation shader to program
		glAttachShader(_programID, _tesselationEvaluationShader);
	}

	// Geometry Shader
	if (_geometryShaderPath.compare("") != 0)
	{
		// Create geometry shader
		_geometryShader = shaderManager.loadShaderFromFile(_geometryShaderPath, GL_GEOMETRY_SHADER);

		// Check for errors
		if (_geometryShader == 0)
		{
			glDeleteProgram(_programID);
			_programID = 0;
			return false;
		}

		// Attach geometry shader to program
		glAttachShader(_programID, _geometryShader);
	}

	// Fragment Shader
	if (_fragmentShaderPath.compare("") != 0)
	{
		// Create fragment shader
		_fragmentShader = shaderManager.loadShaderFromFile(_fragmentShaderPath, GL_FRAGMENT_SHADER);

		// Check for errors
		if (_fragmentShader == 0)
		{
			glDeleteProgram(_programID);
			_programID = 0;
			return false;
		}

		// Attach fragment shader to program
		glAttachShader(_programID, _fragmentShader);
	}

	// Link program
	glLinkProgram(_programID);

	// Check for errors
	GLint programSuccess = GL_FALSE;
	glGetProgramiv(_programID, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		printf("Error linking program %d!\n", _programID);
		PrintProgramLog(_programID);
		glDeleteShader(_vertexShader);
		glDeleteShader(_tesselationControlShader);
		glDeleteShader(_tesselationEvaluationShader);
		glDeleteShader(_geometryShader);
		glDeleteShader(_fragmentShader);
		glDeleteProgram(_programID);
		_programID = 0;
		
		return false;
	}

	// Clean up excess shader references
	glDeleteShader(_vertexShader);
	glDeleteShader(_tesselationControlShader);
	glDeleteShader(_tesselationEvaluationShader);
	glDeleteShader(_geometryShader);
	glDeleteShader(_fragmentShader);

	return true;
}


void ProgramManager::PrintProgramLog(GLuint program)
{
	if (glIsProgram(program))
	{
		// Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		// Get info string length
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// Allocate string
		char* infoLog = new char[maxLength];

		// Get info log
		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			printf("%s\n", infoLog);
		}

		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a program\n", program);
	}
}


GLuint ProgramManager::LoadTexture(const char* imagepath)
{
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3

	// Actual RGB data
	unsigned char * data;

	FILE * file = fopen(imagepath, "rb");
	if (!file)
	{
		printf("Image could not be opened\n");
		return 0;
	}
	if (fread(header, 1, 54, file) != 54)
	{
		// If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return false;
	}
	if (header[0] != 'B' || header[1] != 'M')
	{
		printf("Not a correct BMP file\n");
		return 0;
	}

	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)
		imageSize = width*height * 3;
	if (dataPos == 0)
		dataPos = 54; // The BMP header is done that way

	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return textureID;
}
