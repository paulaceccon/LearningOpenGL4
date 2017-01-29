// Include OpenGL  
#include <glm/gtc/type_ptr.hpp> 

// Include the standard C++ headers  
#include <algorithm>

// Include custom classes
#include "Arcball.h"


Arcball::Arcball(float rollSpeed, int windowWidth, int windowHeight)
{
	_rollSpeed   = rollSpeed;
    _angle       = 0.0;
    _camAxis     = glm::vec3(0.0f, 1.0f, 0.0f);
    _windowWidth  = windowWidth;
    _windowHeight = windowHeight;
}


Arcball::~Arcball()
{

}


const glm::vec3 Arcball::CalculateArcballVector(int x, int y) const
{
	// Convert the mouse cursor coordinate on the window (i.e. from (0,0) to (windowWidth, windowHeight))
 	// into normalized screen coordinate (i.e. (-1, -1) to (1, 1)
	glm::vec3 OP = glm::vec3( (2.0 * x - _windowWidth)  / _windowWidth,
							 -(2.0 * y - _windowHeight) / _windowHeight,
							   0.0);
  	
  	// Clamp it to border of the windows, comment these codes to allow rotation when cursor is not over window 
    OP.x = glm::clamp(OP.x, -1.0f, 1.0f);
    OP.y = glm::clamp(OP.y, -1.0f, 1.0f);
    
  	// Deriving z coordinate
  	float squaredOP = OP.x * OP.x + OP.y * OP.y;
  	if (squaredOP <= 1.0)
    	OP.z = sqrt(1.0 * 1.0 - squaredOP);  
  	else
    	OP = glm::normalize(OP);  
	
	return OP;
}


void Arcball::CalculateRotationAttributes(int lastPosX, int lastPosY, int currPosX, int currPosY)
{
	glm::vec3 OPa = CalculateArcballVector(lastPosX, lastPosY);
    glm::vec3 OPb = CalculateArcballVector(currPosX, currPosY);
    
    // Calculate the angle between OPa and OPb in radians, and clamp it between 0 and 90 degrees
    _angle = acos(std::min(1.0f, glm::dot(OPa, OPb)));
    
    // Cross product to get the rotation axis, but it's still in camera coordinate
    _camAxis = glm::cross(OPa, OPb);
}


glm::mat4 Arcball::GetViewRotationMatrix() 
{
    return glm::rotate(glm::degrees(_angle) * _rollSpeed, _camAxis);
}


glm::mat4 Arcball::GetModelRotationMatrix(const glm::mat4& view)
{
    glm::vec3 axis = glm::inverse(glm::mat3(view)) * _camAxis;
    return glm::rotate(glm::degrees(_angle) * _rollSpeed, axis);
}