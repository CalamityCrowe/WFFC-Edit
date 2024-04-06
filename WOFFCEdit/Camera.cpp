

#include "Camera.h"

#include "InputCommands.h"


Camera::Camera()
{
	m_movespeed = 0.30;
	m_camRotRate = 3.0;

	//camera
	m_camPos.x = 0.0f;
	m_camPos.y = 3.7f;
	m_camPos.z = -3.5f;


	m_camLookAt.x = 0.0f;
	m_camLookAt.y = 0.0f;
	m_camLookAt.z = 0.0f;

	m_camLookDirection.x = 0.0f;
	m_camLookDirection.y = 0.0f;
	m_camLookDirection.z = 0.0f;

	m_camRight.x = 0.0f;
	m_camRight.y = 0.0f;
	m_camRight.z = 0.0f;

	m_camOrientation.x = 0.0f;
	m_camOrientation.y = 0.0f;
	m_camOrientation.z = 0.0f;


	orbitRadius = 5;
	orbitAngle = 0;
	ArcTarget = DirectX::XMFLOAT3(0, 0, 0);

}

void Camera::Update(InputCommands* commands)
{
	//DirectX::SimpleMath::Vector3 planarMotionVector = m_camLookDirection;
	//planarMotionVector.y = 0.0;
	if (commands->arcBall) // if the arc ball is enabled
	{
		m_camRotRate = 0.1f; // set the rotation rate to 0.1
	}
	else
	{
		m_camRotRate = 3.0f; // set the rotation rate to 3.0
	}



	if (commands->RMB == false) // if the right mouse button down
	{
		if (commands->arcBall == false) // if the arcball is not enabled
		{


			float cosR, cosP, cosY; // creates floats for the cosines of the roll, pitch, and yaw
			float sinR, sinP, sinY; // creates floats for the sines of the roll, pitch, and yaw

			cosP = cosf(m_camOrientation.x * (3.1415 / 180)); // sets the cosP based on the pitch of the camera in radians
			cosY = cosf(m_camOrientation.y * (3.1415 / 180)); // sets the cosY based on the yaw of the camera in radians
			cosR = cosf(m_camOrientation.z * (3.1415 / 180)); // sets the cosR based on the roll of the camera in radians

			sinP = sinf(m_camOrientation.x * (3.1415 / 180)); // sets the sinP based on the pitch of the camera in radians
			sinY = sinf(m_camOrientation.y * (3.1415 / 180)); // sets the sinY based on the yaw of the camera in radians 
			sinR = sinf(m_camOrientation.z * (3.1415 / 180)); // sets the sinR based on the roll of the camera in radians

			m_camLookDirection.x = sinY * cosP; // sets the look direction x based on the yaw and pitch
			m_camLookDirection.y = sinP; // sets the look direction y based on the pitch
			m_camLookDirection.z = cosP * -cosY; // sets the look direction z based on the pitch and yaw

			m_camLookDirection.Normalize(); // normalizes the look direction

			//create right vector from look Direction
			m_camLookDirection.Cross(DirectX::SimpleMath::Vector3::UnitY, m_camRight); // creates the right vector from the look direction
		}
		else
		{
			float orbX = orbitRadius * sin(m_camOrientation.y) * cos(m_camOrientation.x); // sets the orbit x position based on the yaw and pitch
			float orbY = orbitRadius * sin(m_camOrientation.x); // sets the orbit y position based on the pitch
			float orbZ = orbitRadius * cos(m_camOrientation.y) * cos(m_camOrientation.x); // sets the orbit z position based on the yaw and pitch
			m_camPos.x = ArcTarget.x + orbX; // sets the camera position x based on the orbit x position and the target x position
			m_camPos.y = ArcTarget.y + orbY; // sets the camera position y based on the orbit y position and the target y position
			m_camPos.z = ArcTarget.z + orbZ; // sets the camera position z based on the orbit z position and the target z position

			m_camLookDirection = ArcTarget - m_camPos; // sets the look direction based on the target position and the camera position
			m_camLookDirection.Normalize(); // normalizes the look direction

			m_camLookAt = ArcTarget; // sets the look at position to the target position


		}
	}

	//apply camera vectors


}

void Camera::HandleMouse(InputCommands* Input)
{
	if (Input) // if the input is not null
	{
		if (Input->manipulateTerrain == false) // if the terrain is not being manipulated
		{



			newMouse = DirectX::SimpleMath::Vector2(Input->mouseX, Input->mouseY); // sets the new mouse position based on the input mouse position

			if (Input->RMB) // if the right mouse button is down
			{
				DirectX::SimpleMath::Vector2 Difference; // creates a vector2 for the difference

				Difference.x = newMouse.x - oldMouse.x; // sets the x difference based on the new mouse x and the old mouse x
				Difference.y = newMouse.y - oldMouse.y; // sets the y difference based on the new mouse y and the old mouse y

				if (Input->arcBall == false) // if the arcball is not enabled
				{
					Difference.Normalize(); // normalizes the difference

					if (Difference.x != 0 || Difference.y != 0) // this will only run if the difference is not 0
					{
						m_camOrientation.y += m_camRotRate * Difference.x; // yaw
						m_camOrientation.x -= m_camRotRate * Difference.y; // pitch
					}

					float cosR, cosP, cosY; // creates floats for the cosines of the roll, pitch, and yaw
					float sinR, sinP, sinY; // creates floats for the sines of the roll, pitch, and yaw

					cosP = cosf(m_camOrientation.x * (3.1415 / 180)); // sets the cosP based on the pitch of the camera in radians
					cosY = cosf(m_camOrientation.y * (3.1415 / 180)); // sets the cosY based on the yaw of the camera in radians
					cosR = cosf(m_camOrientation.z * (3.1415 / 180)); // sets the cosR based on the roll of the camera in radians

					sinP = sinf(m_camOrientation.x * (3.1415 / 180)); // sets the sinP based on the pitch of the camera in radians
					sinY = sinf(m_camOrientation.y * (3.1415 / 180)); // sets the sinY based on the yaw of the camera in radians
					sinR = sinf(m_camOrientation.z * (3.1415 / 180)); // sets the sinR based on the roll of the camera in radians

					m_camLookDirection.x = sinY * cosP; // sets the look direction x based on the yaw and pitch
					m_camLookDirection.y = sinP; // sets the look direction y based on the pitch
					m_camLookDirection.z = cosP * -cosY; // sets the look direction z based on the pitch and yaw

					m_camLookDirection.Cross(DirectX::SimpleMath::Vector3::UnitY, m_camRight); // creates the right vector from the look direction
					m_camLookAt = m_camPos + m_camLookDirection; // sets the look at position based on the camera position and the look direction

				}
				else
				{
					//arcball rotation
					// This needs implemented and needs to know about the selected object
					Difference.Normalize();
					if (Difference.x != 0 || Difference.y != 0) // this will only run if the difference is not 0
					{
						m_camOrientation.y += m_camRotRate * Difference.x; // yaw
						m_camOrientation.x -= m_camRotRate * Difference.y; // pitch
					}

					float orbX = orbitRadius * sin(m_camOrientation.y) * cos(m_camOrientation.x); // sets the orbit x position based on the yaw and pitch
					float orbY = orbitRadius * sin(m_camOrientation.x); // sets the orbit y position based on the pitch
					float orbZ = orbitRadius * cos(m_camOrientation.y) * cos(m_camOrientation.x); // sets the orbit z position based on the yaw and pitch
					m_camPos.x = ArcTarget.x + orbX; // sets the camera position x based on the orbit x position and the target x position
					m_camPos.y = ArcTarget.y + orbY; // sets the camera position y based on the orbit y position and the target y position
					m_camPos.z = ArcTarget.z + orbZ; // sets the camera position z based on the orbit z position and the target z position

					m_camLookDirection = ArcTarget - m_camPos; // sets the look direction based on the target position and the camera position
					m_camLookDirection.Normalize(); // normalizes the look direction

					m_camLookDirection.Cross(DirectX::SimpleMath::Vector3::UnitY, m_camRight); // creates the right vector from the look direction

					m_camLookAt = ArcTarget; // sets the look at position to the target position

				}
			}


			oldMouse = newMouse; // sets the old mouse position based on the new mouse position
		}
	}

}

void Camera::HandleMovement(InputCommands* Input)
{
	if (Input) // if the input is not null
	{
		if (Input->forward)	 // if the forward key is pressed
		{
			m_camPos += m_camLookDirection * m_movespeed; // move the camera forward based on the look direction and the move speed
		}
		if (Input->back) // if the back key is pressed
		{
			m_camPos -= m_camLookDirection * m_movespeed; // move the camera back based on the look direction and the move speed
		}
		if (Input->right) // if the right key is pressed
		{
			m_camPos += m_camRight * m_movespeed; // move the camera right based on the right vector and the move speed
		}
		if (Input->left) // if the left key is pressed
		{
			m_camPos -= m_camRight * m_movespeed; // move the camera left based on the right vector and the move speed
		}
		if (Input->Up)
		{
			m_camPos += DirectX::SimpleMath::Vector3(0, 1, 0) * m_movespeed; // move the camera up based on the move speed
		}
		if (Input->Down)
		{
			m_camPos -= DirectX::SimpleMath::Vector3(0, 1, 0) * m_movespeed; // move the camera down based on the move speed
		}

		if (Input->rotRight) // if the right key is pressed
		{
			m_camOrientation.y += m_camRotRate; // rotate the camera yaw right based on the rotation rate
		}
		if (Input->rotLeft) // if the left key is pressed
		{
			m_camOrientation.y -= m_camRotRate; // rotate the camera yaw left based on the rotation rate
		}
		if (Input->rotUp) // if the up key is pressed
		{
			m_camOrientation.x += m_camRotRate; // rotate the camera pitch up based on the rotation rate
		}
		if (Input->rotDown) // if the down key is pressed
		{
			m_camOrientation.x -= m_camRotRate; // rotate the camera pitch down based on the rotation rate
		}

		if (Input->IncreaseOrbit) // if the increase orbit key is pressed
			orbitRadius += 0.05; // increase the orbit radius by 0.05
		if (Input->DecreaseOrbit) // if the decrease orbit key is pressed
			orbitRadius -= 0.05; // decrease the orbit radius by 0.05


		m_camLookAt = m_camPos + m_camLookDirection; // set the look at position based on the camera position and the look direction
		CreateLookAt(); // create the look at matrix
	}
}
