#pragma once
#include "DisplayChunk.h"
#include "DeviceResources.h"


class InputCommands;
class Camera;

class TerrainManipulator
{
public:
	TerrainManipulator();
	~TerrainManipulator();

	void TerrainManipulation(InputCommands* m_InputCommands, std::vector<std::unique_ptr<Camera>>& m_Cameras, int m_CurrentCamera, DirectX::SimpleMath::Matrix& m_world, DirectX::SimpleMath::Matrix& m_projection);
	DirectX::SimpleMath::Vector3 TerrainIntersection(InputCommands* m_InputCommands, std::vector<std::unique_ptr<Camera>>& m_Cameras, int m_CurrentCamera, DirectX::SimpleMath::Matrix& m_world, DirectX::SimpleMath::Matrix& m_projection);
	void SetScreenDimensions(RECT dimensions) { m_ScreenDimensions = dimensions; }
	void SetDeviceResources(std::shared_ptr<DX::DeviceResources> deviceResources) { m_deviceResources = deviceResources; }

	void HandleInput(InputCommands*);

	void SetDisplayChunk(DisplayChunk& displayChunk) { m_displayChunk = &displayChunk; }

private:
	DisplayChunk* m_displayChunk;

	RECT m_ScreenDimensions;
	std::shared_ptr<DX::DeviceResources>    m_deviceResources;
	float inRadius = 5;
	float outRadius = 10;
	float moveAmount = 0.25f;
	float lowerClamp = 0;
	float upperClamp = 64;
};

