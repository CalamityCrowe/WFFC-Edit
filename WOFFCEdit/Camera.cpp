

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
	if (commands->arcBall)
	{
		m_camRotRate = 0.1f;
	}
	else
	{
		m_camRotRate = 3.0f;
	}

	if (commands->rotRight)
	{
		m_camOrientation.y += m_camRotRate;
	}
	if (commands->rotLeft)
	{
		m_camOrientation.y -= m_camRotRate;
	}
	if (commands->rotUp)
	{
		m_camOrientation.x += m_camRotRate;
	}
	if (commands->rotDown)
	{
		m_camOrientation.x -= m_camRotRate;
	}


	if (commands->RMB == false)
	{
		if (commands->arcBall == false)
		{


			float cosR, cosP, cosY;
			float sinR, sinP, sinY;

			cosP = cosf(m_camOrientation.x * (3.1415 / 180));
			cosY = cosf(m_camOrientation.y * (3.1415 / 180));
			cosR = cosf(m_camOrientation.z * (3.1415 / 180));

			sinP = sinf(m_camOrientation.x * (3.1415 / 180));
			sinY = sinf(m_camOrientation.y * (3.1415 / 180));
			sinR = sinf(m_camOrientation.z * (3.1415 / 180));

			m_camLookDirection.x = sinY * cosP;
			m_camLookDirection.y = sinP;
			m_camLookDirection.z = cosP * -cosY;

			m_camLookDirection.Normalize();

			//create right vector from look Direction
			m_camLookDirection.Cross(DirectX::SimpleMath::Vector3::UnitY, m_camRight);

			//process input and update stuff


			//update lookat point
			if (commands->forward)
			{
				m_camPos += m_camLookDirection * m_movespeed;
			}
			if (commands->back)
			{
				m_camPos -= m_camLookDirection * m_movespeed;
			}
			if (commands->right)
			{
				m_camPos += m_camRight * m_movespeed;
			}
			if (commands->left)
			{
				m_camPos -= m_camRight * m_movespeed;
			}
			m_camLookAt = m_camPos + m_camLookDirection;
		}
		else
		{
			float orbX = orbitRadius * sin(m_camOrientation.y) * cos(m_camOrientation.x);
			float orbY = orbitRadius * sin(m_camOrientation.x);
			float orbZ = orbitRadius * cos(m_camOrientation.y) * cos(m_camOrientation.x);
			m_camPos.x = ArcTarget.x + orbX;
			m_camPos.y = ArcTarget.y + orbY;
			m_camPos.z = ArcTarget.z + orbZ;

			m_camLookDirection = ArcTarget - m_camPos;
			m_camLookDirection.Normalize();

			m_camLookAt = ArcTarget;


		}
	}

	//apply camera vectors


}

void Camera::HandleMouse(InputCommands* Input)
{
	if (Input)
	{
		if (Input->manipulateTerrain == false)
		{



			newMouse = DirectX::SimpleMath::Vector2(Input->mouseX, Input->mouseY);

			if (Input->RMB)
			{
				DirectX::SimpleMath::Vector2 Difference;

				Difference.x = newMouse.x - oldMouse.x;
				Difference.y = newMouse.y - oldMouse.y;

				if (Input->arcBall == false)
				{
					Difference.Normalize();

					if (Difference.x != 0 || Difference.y != 0) // this will only run if the difference is not 0
					{
						m_camOrientation.y += m_camRotRate * Difference.x; // yaw
						m_camOrientation.x -= m_camRotRate * Difference.y; // pitch
					}

					float cosR, cosP, cosY;
					float sinR, sinP, sinY;

					cosP = cosf(m_camOrientation.x * (3.1415 / 180));
					cosY = cosf(m_camOrientation.y * (3.1415 / 180));
					cosR = cosf(m_camOrientation.z * (3.1415 / 180));

					sinP = sinf(m_camOrientation.x * (3.1415 / 180));
					sinY = sinf(m_camOrientation.y * (3.1415 / 180));
					sinR = sinf(m_camOrientation.z * (3.1415 / 180));

					m_camLookDirection.x = sinY * cosP;
					m_camLookDirection.y = sinP;
					m_camLookDirection.z = cosP * -cosY;

					m_camLookDirection.Cross(DirectX::SimpleMath::Vector3::UnitY, m_camRight);
					m_camLookAt = m_camPos + m_camLookDirection;

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

					float orbX = orbitRadius * sin(m_camOrientation.y) * cos(m_camOrientation.x);
					float orbY = orbitRadius * sin(m_camOrientation.x);
					float orbZ = orbitRadius * cos(m_camOrientation.y) * cos(m_camOrientation.x);
					m_camPos.x = ArcTarget.x + orbX;
					m_camPos.y = ArcTarget.y + orbY;
					m_camPos.z = ArcTarget.z + orbZ;

					m_camLookDirection = ArcTarget - m_camPos;
					m_camLookDirection.Normalize();

					m_camLookDirection.Cross(DirectX::SimpleMath::Vector3::UnitY, m_camRight);

					m_camLookAt = ArcTarget;

				}
			}


			oldMouse = newMouse;
		}
	}

}

void Camera::HandleMovement(InputCommands* Input)
{
	if (Input)
	{
		if (Input->forward)
		{
			m_camPos += m_camLookDirection * m_movespeed;
		}
		if (Input->back)
		{
			m_camPos -= m_camLookDirection * m_movespeed;
		}
		if (Input->right)
		{
			m_camPos += m_camRight * m_movespeed;
		}
		if (Input->left)
		{
			m_camPos -= m_camRight * m_movespeed;
		}
		if (Input->IncreaseOrbit)
			orbitRadius += 0.05;
		if (Input->DecreaseOrbit)
			orbitRadius -= 0.05;


		m_camLookAt = m_camPos + m_camLookDirection;
		CreateLookAt();
	}
}
