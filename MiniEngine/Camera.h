#pragma once
class Camera
{
public:
	float eye[3];
	float lookat[3];
	float up[3];
	double prevMouseX, prevMouseY;
	bool mouseLeftPressed;
	bool mouseMiddlePressed;
	bool mouseRightPressed;
	float curr_quat[4];
	float prev_quat[4];
	Camera();
	~Camera();
};

