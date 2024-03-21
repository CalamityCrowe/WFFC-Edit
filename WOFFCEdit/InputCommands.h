#pragma once

struct InputCommands
{
	bool forward;
	bool back;
	bool right;
	bool left;
	bool rotRight;
	bool rotLeft;
	bool rotUp;
	bool rotDown;

	bool focus;
	bool arcBall; // false will be free rotation, true will be the arcing rotation

	float mouseX;
	float mouseY;

	bool IncreaseOrbit;
	bool DecreaseOrbit;


	bool RMB;
	bool LMB;

	int CameraSelected = 0;
};
