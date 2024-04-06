#pragma once

struct InputCommands
{
	bool forward;
	bool back;
	bool right;
	bool left;
	bool Up;
	bool Down;
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

	bool copy;
	bool paste; 
	bool undo;
	bool redo;
	bool deleteSelected;

	int CameraSelected = 0;

	float terrainDir; 
	bool manipulateTerrain; 

	bool IncreaseBrushSize;
	bool DecreaseBrushSize;
	bool IncreaseBrushStrength;
	bool DecreaseBrushStrength;
	bool IncreaseClamp; 
	bool DecreaseClamp;
};
