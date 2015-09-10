#pragma once

// Include OpenGL
#include <GL/glew.h> 

// Include the standard C++ headers 
#include <string>

class ProgramManager
{
public:
	/**
	*  Default constructor.
	*
	*  \param vertexShaderPath : The vertex shader path.
	*  \param fragmentShaderPath : The fragment shader path.
	**/
	ProgramManager();

	/**
	*  Default destructor.
	*/
	~ProgramManager();

	/**
	*  Create a program.
	*
	*  \return : true, if the program is properly created; false, otherwise.
	**/
	bool CreateProgram();

	/**
	*  Free the program, if it exists.
	**/
	void FreeProgram();

	/**
	*  Set the class program as the current shader program.
	*
	*  \return : true, if it succeeds; false, otherwise.
	**/
	bool Bind();

	/**
	*  Set a default program as the current shader program.
	**/
	void UnBind();

	/**
	*  Get the program ID.
	*
	*  \return : The program ID.
	**/
	GLuint GetProgramID();

	/**
	*  Set the vertex shader path. 
	**/
	void SetVertexShader(const std::string vertexShaderPath);

	/**
	*  Set the fragment shader path.
	**/
	void SetFragmentShader(const std::string fragmentShaderPath);

	/**
	*  Set the tesselation control shader path.
	**/
	void SetTesselationControlShader(const std::string tesselationControlPath);
	
	/**
	*  Set the tesselation evaluation shader path.
	**/
	void SetTesselationEvaluationShader(const std::string tesselationEvaluationPath);

	/**
	*  Set the geometry evaluation shader path.
	**/
	void SetGeometryShader(const std::string geometryShaderPath);

	/**
	*  Load all attribute variables that a shader program need to run.
	**/
	virtual void LoadAttributeVariables() = 0;

	/**
	*  Load all uniform variables that a shader program need to run.
	**/
	virtual void LoadUniformVariables() = 0;

private:
	/**
	*  Prints program log.
	*
	*  \param program : The program ID.
	**/
	void PrintProgramLog(GLuint program);

	// The program ID.
	GLuint _programID;

	// The vertex shader path.
	std::string _vertexShaderPath;

	// The fragment shader path.
	std::string _fragmentShaderPath;

	// The tesselation control shader path.
	std::string _tesselationControlPath;

	// The tesselation evaluation shader path.
	std::string _tesselationEvaluationPath;

	// The geometry shader path.
	std::string _geometryShaderPath;

	// The shaders IDs.
	GLuint _vertexShader, _fragmentShader, _tesselationControlShader, _tesselationEvaluationShader, _geometryShader;
};

