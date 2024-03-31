#include "TerrainManipulator.h"
#include "InputCommands.h"
#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

TerrainManipulator::TerrainManipulator()
{
}

TerrainManipulator::~TerrainManipulator()
{
}



void TerrainManipulator::TerrainManipulation(InputCommands* m_InputCommands, std::vector<std::unique_ptr<Camera>> m_Cameras, int m_CurrentCamera,
	DirectX::SimpleMath::Matrix m_world, DirectX::SimpleMath::Matrix m_projection)
{
	Vector3 TerrainIntersectionPoint = TerrainIntersection(m_InputCommands, m_Cameras, m_CurrentCamera, m_world, m_projection);	//get the intersection point of the terrain

	for (int i = 0; i < TERRAINRESOLUTION; ++i) // loops through for the size of the terrain
	{
		for (int j = 0; j < TERRAINRESOLUTION; ++j) // loops through for the size of the terrain
		{
			float distance = Vector3::Distance(Vector3(TerrainIntersectionPoint.x, 0, TerrainIntersectionPoint.z), Vector3(m_displayChunk.m_terrainGeometry[i][j].position.x, 0, m_displayChunk.m_terrainGeometry[i][j].position.z)); // get the distance between the intersection point and the terrain point

			if (distance < outRadius) // if the distance is less than the out radius
			{
				if (distance < inRadius) // if the distance is less than the in radius
				{
					m_displayChunk.m_terrainGeometry[i][j].position.y += moveAmount * m_InputCommands->terrainDir; // move the vertex up or down based on the terrainDir
				}
				else
				{
					m_displayChunk.m_terrainGeometry[i][j].position.y += moveAmount * m_InputCommands->terrainDir * (1 - ((distance - inRadius) / 10)); // move the vertex up or down based on the terrainDir and the distance from the in radius
				}
				//keep vertex within bounds of height map
				if (m_displayChunk.m_terrainGeometry[i][j].position.y < lowerClamp)
					m_displayChunk.m_terrainGeometry[i][j].position.y = lowerClamp;
				else if (m_displayChunk.m_terrainGeometry[i][j].position.y > upperClamp)
					m_displayChunk.m_terrainGeometry[i][j].position.y = upperClamp;

			}
		}
	}
}

DirectX::SimpleMath::Vector3 TerrainManipulator::TerrainIntersection(InputCommands* m_InputCommands, std::vector<std::unique_ptr<Camera>> m_Cameras, int m_CurrentCamera, DirectX::SimpleMath::Matrix m_world, DirectX::SimpleMath::Matrix m_projection)
{

	Vector3 intLoc = Vector3::Zero;
	bool hasIntersected = false;
	//setup near and far planes of frustum with mouse X and mouse y passed down from Toolmain. 
	//they may look the same but note, the difference in Z
	const XMVECTOR nearSource = XMVectorSet(m_InputCommands->mouseX, m_InputCommands->mouseY, 0.0f, 1.0f);
	const XMVECTOR farSource = XMVectorSet(m_InputCommands->mouseX, m_InputCommands->mouseY, 1.0f, 1.0f);

	XMVECTOR nearPoint = XMVector3Unproject(nearSource, 0.0f, 0.0f, m_ScreenDimensions.right, m_ScreenDimensions.bottom, m_deviceResources->GetScreenViewport().MinDepth, m_deviceResources->GetScreenViewport().MaxDepth, m_projection, m_Cameras[m_CurrentCamera]->GetView(), m_world);
	XMVECTOR farPoint = XMVector3Unproject(farSource, 0.0f, 0.0f, m_ScreenDimensions.right, m_ScreenDimensions.bottom, m_deviceResources->GetScreenViewport().MinDepth, m_deviceResources->GetScreenViewport().MaxDepth, m_projection, m_Cameras[m_CurrentCamera]->GetView(), m_world);

	XMVECTOR direction = XMVector3Normalize(farPoint - nearPoint); //get the direction of the ray

	for (size_t i = 0; i < TERRAINRESOLUTION - 1; ++i) //loop through till it hits the terrain resolution
	{
		if (hasIntersected) // checks if it has intersected with the terrain and if so breaks from the loop
		{
			break;
		}
		for (size_t j = 0; j < TERRAINRESOLUTION - 1; ++j) //loop through till it hits the terrain resolution
		{
			//get the 4 corners of the quad
			XMVECTOR v0 = XMLoadFloat3(&m_displayChunk.m_terrainGeometry[i][j].position);
			XMVECTOR v1 = XMLoadFloat3(&m_displayChunk.m_terrainGeometry[i][j + 1].position);
			XMVECTOR v2 = XMLoadFloat3(&m_displayChunk.m_terrainGeometry[i + 1][j].position);
			XMVECTOR v3 = XMLoadFloat3(&m_displayChunk.m_terrainGeometry[i + 1][j + 1].position);

			//check for intersection with the quad

			XMVECTOR normal = XMVector3Normalize(XMVector3Cross(v1 - v0, v2 - v0)); // get the normal of the quad based off of the 3 points cross product
			float distance = -XMVectorGetX(XMVector3Dot(normal, v0)); // get the distance of the plane from the origin
			XMVECTOR plane = XMVectorSetW(normal, distance); // set the distance of the plane

			XMVECTOR Intersect = XMPlaneIntersectLine(plane, nearPoint, farPoint); //get the intersection point

			if (!XMVector3Equal(Intersect, XMVectorZero())) //if the intersection point is not zero
			{
				Vector3 intersectPoint; // create a vector3 to store the intersect point
				XMStoreFloat3(&intersectPoint, Intersect); // store the intersect point in the vector3

				if (intersectPoint.x >= std::min(XMVectorGetX(v0), XMVectorGetX(v1)) &&
					intersectPoint.x <= std::max(XMVectorGetX(v0), XMVectorGetX(v1)) &&
					intersectPoint.z >= std::min(XMVectorGetX(v0), XMVectorGetX(v3)) &&
					intersectPoint.z <= std::max(XMVectorGetX(v0), XMVectorGetX(v3))) // check if the intersect point is within the quad
				{
					intLoc = intersectPoint; // set the intersect point to the intLoc 
					hasIntersected = true; // set hasIntersected to true
					break; // break from the loop
				}
			}
		}
	}
	if (hasIntersected) // if it has intersected
	{
		m_displayChunk.CalculateTerrainNormals(); // calculate the terrain normals
		return intLoc; // return the intersect point
	}
	else
	{
		return Vector3(-99999, -99999, -99999); // return a vector3 with -99999, -99999, -99999
	}


}

void TerrainManipulator::HandleInput(InputCommands*)
{
	//this is where you would handle input for the terrain manipulation tool
	//you can add more functionality here if you wish
}
