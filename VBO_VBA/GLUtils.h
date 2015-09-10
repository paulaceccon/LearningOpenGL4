#ifndef _GLUTILS_H_
#define _GLUTILS_H_

#include <GL/glew.h>

#include <iostream>

#define CheckOpenGLError() CheckGLError(__FILE__,__LINE__)


// To check OpenGL errors
inline void CheckGLError(const char *text, unsigned int value)
{
	GLenum result;
	result = glGetError();
	if (result)
	{
		unsigned int index = 0;
		while (result != GL_NO_ERROR)
		{
			switch (result)
			{
			case GL_INVALID_ENUM:
				printf("(%u) %s:%u-GL_INVALID_ENUM\n", index, text, value);
				break;
			case GL_INVALID_VALUE:
				printf("(%u) %s:%u-GL_INVALID_VALUE\n", index, text, value);
				break;
			case GL_INVALID_OPERATION:
				printf("(%u) %s:%u-GL_INVALID_OPERATION\n", index, text, value);
				break;
			case GL_STACK_OVERFLOW:
				printf("(%u) %s:%u-GL_STACK_OVERFLOW\n", index, text, value);
				break;
			case GL_STACK_UNDERFLOW:
				printf("(%u) %s:%u-GL_STACK_UNDERFLOW\n", index, text, value);
				break;
			case GL_OUT_OF_MEMORY:
				printf("(%u) %s:%u-GL_OUT_OF_MEMORY\n", index, text, value);
				break;
			default:
				printf("(%u) %s:%u-UNHANDLED ERROR ENUM %d\n", index, text, value, result);
				break;
			}
			++index;
			result = glGetError();
		}
	}
}

#endif