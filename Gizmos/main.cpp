// Include OpenGL  
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Include the standard C++ headers  
#include <stack>

// Include custom C++ headers
#include "../Common/GLUtils.h"
#include "CubeShaderProgram.h"
#include "GizmosShaderProgram.h"

#define PI 3.141592f  /* pi */


// Interaction
GLfloat alpha = 0.0f, beta = 0.0f;
GLfloat zoom = 1.0f;
GLint width = 640, height = 640;


// Define the model view matrix stack
std::stack<glm::mat4> glm_ModelViewMatrix;

// Set the perspective matrix
glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width / height, 1.0f, 100.f);


// Define a GLFW error callback  
static void ErrorCallback(int error, const char* description)
{
	fputs(description, stderr);
	getchar();
}


// Define a GLFW key callback
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS)
		return;

	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_LEFT:
		alpha += 2.0f * PI / 20;
		break;
	case GLFW_KEY_RIGHT:
		alpha -= 2.0f * PI / 20;
		break;
	case GLFW_KEY_UP:
		beta -= 2.0f * PI / 20;
		break;
	case GLFW_KEY_DOWN:
		beta += 2.0f * PI / 20;
		break;
	case GLFW_KEY_PAGE_UP:
		zoom -= 0.25f;
		break;
	case GLFW_KEY_PAGE_DOWN:
		zoom += 0.25f;
		break;

	default:
		break;
	}
}


// Define a GLFW scrooll whell callback
static void ScrollCallback(GLFWwindow* window, double x, double y)
{
	zoom += (float)y / 4.f;
}


// Define a GLFW resize callback
static void WindowSizeCallback(GLFWwindow* window, int w, int h)
{
	glfwGetFramebufferSize(window, &width, &height);
    Projection = glm::perspective(glm::radians(45.0f), (float)width / height, 1.0f, 100.f);
}


// Shows FPS as window title
void UpdateFPS(GLFWwindow* window) 
{
	static double previous_seconds = glfwGetTime();
  	static int frame_count;
  
  	double current_seconds = glfwGetTime();
  	double elapsed_seconds = current_seconds - previous_seconds;
  
  	if (elapsed_seconds > 0.25) 
  	{
    	previous_seconds = current_seconds;
    	double fps = (double)frame_count / elapsed_seconds;
    	char tmp[128];
    	sprintf(tmp, "opengl @ fps: %.2f", fps);
    	glfwSetWindowTitle(window, tmp);
    	frame_count = 0;
  	}
  	frame_count++;
}


int main(void)
{
	// Set the error callback  
	glfwSetErrorCallback(ErrorCallback);

	// Initialize GLFW  
	if (!glfwInit())
	{
		printf("Error initializing GLFW!\n");
		exit(EXIT_FAILURE);
	}

	// Set the GLFW window creation hints - these are optional  
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);		 // Request a specific OpenGL version  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);		 // Request a specific OpenGL version  
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Compatibility with OpenGL4
	glfwWindowHint(GLFW_SAMPLES, 4);					 //Antialising level

	// Create a window and create its OpenGL context    
	GLFWwindow* window = glfwCreateWindow(width, height, "Learning OpenGL 4", NULL, NULL);

	// If the window couldn't be created  
	if (!window)
	{
		printf("Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Sets the context of the specified window on the calling thread  
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true;	// To prevent a known error, not working
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK && glewError != GL_INVALID_ENUM)
	{
		printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
		glfwDestroyWindow(window);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	// Just to consume the GLEW GL_INVALID_ENUM error 
	// (http://stackoverflow.com/questions/20822087/why-does-this-crash-when-using-opengl-core-profile)
	CheckOpenGLError();

	// Set the window callbacks
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetWindowSizeCallback(window, WindowSizeCallback);
	glfwSetScrollCallback(window, ScrollCallback);
	glfwGetFramebufferSize(window, &width, &height);

	// Set the view matrix
	glm::mat4 ModelView = glm::lookAt(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Initialize matrix stack
	glm_ModelViewMatrix.push(ModelView);

	// Set a background color  
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	// 3D objects
	glEnable(GL_DEPTH_TEST);

	// Shader Program
	CubeShaderProgram cubeShader;
	cubeShader.SetVertexShader("CubeVertexShader.txt");
	cubeShader.SetFragmentShader("CubeFragShader.txt");
	cubeShader.CreateShaderProgram();	
	
	GizmosShaderProgram gizmosShader;
	gizmosShader.SetVertexShader("GizmosVertexShader.txt");
	gizmosShader.SetFragmentShader("GizmosFragShader.txt");
	gizmosShader.CreateShaderProgram();							

	// Main Loop  
	while (!glfwWindowShouldClose(window))
	{
		UpdateFPS(window);
		glViewport(0, 0, width, height);
		
		// Clear color buffer  
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Clone current modelview matrix, which can now be modified 
		glm_ModelViewMatrix.push(glm_ModelViewMatrix.top());
		{
			//------- ModelView Transformations
			// Cube
			glm_ModelViewMatrix.push(glm_ModelViewMatrix.top());
			{
				// Rotation
				glm_ModelViewMatrix.top() = glm::rotate(glm_ModelViewMatrix.top(), beta, glm::vec3(1.0, 0.0, 0.0));
				glm_ModelViewMatrix.top() = glm::rotate(glm_ModelViewMatrix.top(), alpha, glm::vec3(0.0, 0.0, 1.0));
				// Zoom in/out
				zoom = (zoom < 0.25) ? 0.25 : zoom;
				glm_ModelViewMatrix.top() = glm::scale(glm_ModelViewMatrix.top(), glm::vec3(zoom, zoom, zoom));
				
				cubeShader.DrawModel(Projection, glm_ModelViewMatrix.top());
			}
			glm_ModelViewMatrix.pop();
			
			// Gizmos
			glm_ModelViewMatrix.push(glm_ModelViewMatrix.top());
			{
				// Translation
				glm_ModelViewMatrix.top() = glm::translate(glm_ModelViewMatrix.top(), glm::vec3(-4.0, -4.0, 0.0));
				// Rotation
				glm_ModelViewMatrix.top() = glm::rotate(glm_ModelViewMatrix.top(), beta, glm::vec3(1.0, 0.0, 0.0));
				glm_ModelViewMatrix.top() = glm::rotate(glm_ModelViewMatrix.top(), alpha, glm::vec3(0.0, 0.0, 1.0));
				
				gizmosShader.DrawModel(Projection, glm_ModelViewMatrix.top());
				
			}
			glm_ModelViewMatrix.pop();
		}
		glm_ModelViewMatrix.pop();

		// Swap buffers  
		glfwSwapBuffers(window);

		// Get and organize events, like keyboard and mouse input, window resizing, etc...  
		glfwPollEvents();

		CheckOpenGLError();
	}

	// Close OpenGL window and terminate GLFW  
	glfwDestroyWindow(window);
	// Finalize and clean up GLFW  
	glfwTerminate();

	exit(EXIT_SUCCESS);
}