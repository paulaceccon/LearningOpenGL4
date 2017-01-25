// Include OpenGL
#include <glm/gtc/type_ptr.hpp> 

// Include custom C++ headers
#include "TesselationShaderProgram.h"
#include "../Common/SimpleOBJLoader.h"


TesselationShaderProgram::TesselationShaderProgram()
{
	_vao = 0;
	_vbo[0] = 0;
	_vbo[1] = 0;
	_ibo = 0;
}


TesselationShaderProgram::~TesselationShaderProgram()
{
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(2, _vbo);
	glDeleteBuffers(1, &_ibo);
}


void TesselationShaderProgram::CreateShaderProgram(const std::string modelPath)
{
	_succeed = CreateProgram();
	if (_succeed)
	{
		BuildModel(modelPath);
		BuildVAO();
	}
}


void TesselationShaderProgram::DrawModel(const glm::mat4 &projection, const glm::mat4 &modelView)
{
	_modelView = modelView;
	_projection = projection;

	_succeed = Bind();
	if (_succeed)
	{
		glBindVertexArray(_vao);
		LoadUniformVariables();
		glPatchParameteri(GL_PATCH_VERTICES, 3);
		glDrawElements(GL_PATCHES, _modelTriangulation.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	UnBind();
}


void TesselationShaderProgram::BuildModel(const std::string modelPath)
{
	SimpleOBJLoader objLoader;
	objLoader.ReadOBJ(modelPath.c_str(), _modelCoordinates, _modelNormals, _modelTriangulation);
}


void TesselationShaderProgram::BuildVAO()
{
	// Generate and bind the vertex array object
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// Generate and bind the vertex buffer object
	glGenBuffers(2, _vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, _modelCoordinates.size() * sizeof(float), &_modelCoordinates[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, _modelNormals.size() * sizeof(float), &_modelNormals[0], GL_STATIC_DRAW);

	// Generate and bind the index buffer object
	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _modelTriangulation.size() * sizeof(unsigned int), &_modelTriangulation[0], GL_STATIC_DRAW);

	LoadAttributeVariables();

	glBindVertexArray(0);
}


void TesselationShaderProgram::LoadUniformVariables()
{
	glm::mat4 MV = _modelView;
	glm::mat4 P = _projection;
	glm::mat3 N = glm::transpose(glm::inverse(glm::mat3(MV)));
	glm::vec4 AC = glm::vec4(0.2, 0.2, 0.2, 1.0);
	glm::vec4 DC = glm::vec4(0.999, 0.5, 0.0, 1.0);
	glm::vec4 SC = glm::vec4(0.3, 0.3, 0.3, 1.0);
	glm::vec3 LP = glm::vec3(0.0, 4.0, 2.0);

	// OpenGL Matrices 
	GLuint ModelView_location = glGetUniformLocation(GetProgramID(), "mvMatrix");
	glUniformMatrix4fv(ModelView_location, 1, GL_FALSE, glm::value_ptr(MV));

	GLuint Projection_location = glGetUniformLocation(GetProgramID(), "pMatrix");
	glUniformMatrix4fv(Projection_location, 1, GL_FALSE, glm::value_ptr(P));

	GLuint Normal_location = glGetUniformLocation(GetProgramID(), "normalMatrix");
	glUniformMatrix3fv(Normal_location, 1, GL_FALSE, glm::value_ptr(N));

	// Tessellation Level
	GLuint TessLevel_location = glGetUniformLocation(GetProgramID(), "tesselletaionLevel");
	glUniform1fv(TessLevel_location, 1, &_tessLevel);

	// Wireframe Mode
	GLuint Wireframe_location = glGetUniformLocation(GetProgramID(), "wireframe");
	glUniform1iv(Wireframe_location, 1, &_wireframeOn);

	// Interpolation Mode
	GLuint InterpolationMode_location = glGetUniformLocation(GetProgramID(), "interpolationMode");
	glUniform1iv(InterpolationMode_location, 1, &_interpolationMode);

	// Lighting 
	GLuint AmbientColor_location = glGetUniformLocation(GetProgramID(), "ambientColor");
	glUniform4fv(AmbientColor_location, 1, glm::value_ptr(AC));

	GLuint DiffuseColor_location = glGetUniformLocation(GetProgramID(), "diffuseColor");
	glUniform4fv(DiffuseColor_location, 1, glm::value_ptr(DC));

	GLuint SpecularColor_location = glGetUniformLocation(GetProgramID(), "specularColor");
	glUniform4fv(SpecularColor_location, 1, glm::value_ptr(SC));

	GLuint LightPosition_location = glGetUniformLocation(GetProgramID(), "vLightPosition");
	glUniform3fv(LightPosition_location, 1, glm::value_ptr(LP));
}


void TesselationShaderProgram::LoadAttributeVariables()
{
	// Vertex Attributes
	GLuint VertexPosition_location = glGetAttribLocation(GetProgramID(), "vPosition");
	glEnableVertexAttribArray(VertexPosition_location);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glVertexAttribPointer(VertexPosition_location, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint VertexNormal_location = glGetAttribLocation(GetProgramID(), "vNormal");
	glEnableVertexAttribArray(VertexNormal_location);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
	glVertexAttribPointer(VertexNormal_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
}


void TesselationShaderProgram::SetTessellationLevel(float tessLevel)
{
	_tessLevel = tessLevel;
}


void TesselationShaderProgram::SetWireframeMode(int wireframeMode)
{
	_wireframeOn = wireframeMode;
}


void TesselationShaderProgram::SetInterpolationMode(int interpolationMode)
{
	_interpolationMode = interpolationMode;
}
