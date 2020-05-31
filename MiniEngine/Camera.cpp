#include "Camera.h"



Camera::Camera()
{
	eye[0] = 0.0f;
	eye[1] = 0.0f;
	eye[2] = 3.0f;

	lookat[0] = 0.0f;
	lookat[1] = 0.0f;
	lookat[2] = 0.0f;

	up[0] = 0.0f;
	up[1] = 1.0f;
	up[2] = 0.0f;
}


Camera::~Camera()
{
}
