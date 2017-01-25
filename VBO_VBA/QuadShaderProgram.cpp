// Include OpenGL
#include <glm/gtc/type_ptr.hpp> 

// Include custom C++ headers
#include "QuadShaderProgram.h"


QuadShaderProgram::QuadShaderProgram()
{
	_vao = 0;
	_vbo[0] = 0;
	_vbo[1] = 0;
}


QuadShaderProgram::~QuadShaderProgram()
{
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(2, _vbo);
}


void QuadShaderProgram::CreateShaderProgram()
{
	_succeed = CreateProgram();
	if (_succeed)
	{
		BuildModel();
		BuildVAO();
	}
}


void QuadShaderProgram::DrawModel(const glm::mat4 &projection, const glm::mat4 &modelView)
{
	_modelView = modelView;
	_projection = projection;

	_succeed = Bind();
	if (_succeed)
	{
		glBindVertexArray(_vao);
		LoadUniformVariables();
		glDrawArrays(GL_TRIANGLES, 0, _modelCoordinates.size());
		glBindVertexArray(0);
	}
	UnBind();
}


void QuadShaderProgram::BuildModel()
{
	_modelCoordinates = { 1.0f,  1.0f, 0.0f,
		                  1.0f, -1.0f, 0.0f,
		                 -1.0f,  1.0f, 0.0f,
						 -1.0f, 1.0f, 0.0f,
						  1.0f, -1.0f, 0.0f,
						 -1.0f, -1.0f, 0.0f };

	_modelColors = { 1.0f, 0.0f, 0.0f,
					 0.0f, 0.0f, 1.0f, 
					 0.0f, 1.0f, 0.0f, 
					 0.0f, 1.0f, 0.0f, 
					 0.0f, 0.0f, 1.0f, 
					 0.0f, 1.0f, 0.0f };
}


void QuadShaderProgram::BuildVAO()
{
	// Generate and bind the vertex array object
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// Generate and bind the vertex buffer object
	glGenBuffers(2, _vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, _modelCoordinates.size() * sizeof(float), &_modelCoordinates[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, _modelColors.size() * sizeof(float), &_modelColors[0], GL_STATIC_DRAW);

	LoadAttributeVariables();

	glBindVertexArray(0);
}


void QuadShaderProgram::LoadUniformVariables()
{
	glm::mat4 MV = _modelView;
	glm::mat4 P = _projection;

	// OpenGL Matrices 
	GLuint ModelView_location = glGetUniformLocation(GetProgramID(), "mvMatrix");
	glUniformMatrix4fv(ModelView_location, 1, GL_FALSE, glm::value_ptr(MV));

	GLuint Projection_location = glGetUniformLocation(GetProgramID(), "pMatrix");
	glUniformMatrix4fv(Projection_location, 1, GL_FALSE, glm::value_ptr(P));
}


void QuadShaderProgram::LoadAttributeVariables()
{
	// Vertex Attributes
	GLuint VertexPosition_location = glGetAttribLocation(GetProgramID(), "vPosition");
	glEnableVertexAttribArray(VertexPosition_location);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glVertexAttribPointer(VertexPosition_location, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint VertexColor_location = glGetAttribLocation(GetProgramID(), "vColor");
	glEnableVertexAttribArray(VertexColor_location);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
	glVertexAttribPointer(VertexColor_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
}