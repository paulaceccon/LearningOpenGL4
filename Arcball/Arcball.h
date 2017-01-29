#ifndef _ARCBALL_H_
#define _ARCBALL_H_

// Include the standard C++ headers  
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>


class Arcball 
{
public:
	/**
	*  Default constructor.
	**/
	Arcball(float rollSpeed, int windowWidth, int windowHeight);

	/**
	*  Default destructor.
	**/
	~Arcball();

	/**
 	*  Get a normalized vector from the center of the virtual ball O to a
 	*  point P on the virtual ball surface, such that P is aligned on
 	*  screen's (X,Y) coordinates.  If (X,Y) is too far away from the
 	*  sphere, return the nearest point on the virtual ball surface.
 	*
 	*  \param x : x coordinate in screen's point
 	*  \param y : y coordinate in screen's point
    * 
    *  \return : Nearest point on the virtual ball surface.
 	**/
	const glm::vec3 CalculateArcballVector(int x, int y) const;

	/**
	*  Create the cube shader program.
	**/
	void CalculateRotationAttributes(int lastPosX, int lastPosY, int currPosX, int currPosY);
	
	/**
 	*  Create rotation matrix within the camera coordinate,
 	*  multiply this matrix with view matrix to rotate the camera
 	**/
	glm::mat4 GetViewRotationMatrix();

	/**
	*  Create rotation matrix within the world coordinate,
	*  multiply this matrix with model matrix to rotate the object
	**/
	glm::mat4 GetModelRotationMatrix(const glm::mat4& view);
	

private:
	// Rotation speed and angle of rotation
	float _rollSpeed, _angle;

	// Rotation axis
	glm::vec3 _camAxis;
	
	// Window's width and height
	int _windowWidth, _windowHeight;
};

#endif