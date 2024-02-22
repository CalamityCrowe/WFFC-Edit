

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

}

void Camera::Update(InputCommands* commands)
{
	//DirectX::SimpleMath::Vector3 planarMotionVector = m_camLookDirection;
	//planarMotionVector.y = 0.0;

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

	//update lookat point
	m_camLookAt = m_camPos + m_camLookDirection;

	//apply camera vectors
	

}

void Camera::HandleMouse(InputCommands* Input)
{
	if (Input)
	{

		newMouse = DirectX::SimpleMath::Vector2(Input->mouseX, Input->mouseY);

		if (Input->LMB)
		{
			DirectX::SimpleMath::Vector2 Difference;

			Difference.x = newMouse.x - oldMouse.x;
			Difference.y = newMouse.y - oldMouse.y;

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

			m_camLookAt = m_camPos + m_camLookDirection;

			m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_camPos, m_camLookAt, DirectX::SimpleMath::Vector3::UnitY);

		}


		oldMouse = newMouse;
	}

}