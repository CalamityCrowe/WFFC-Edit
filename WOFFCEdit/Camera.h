#pragma once


#include "DisplayChunk.h"


class InputCommands; 

class Camera
{
public:
	Camera();

	void Update(InputCommands* commands);

	void HandleMouse(InputCommands* Input);

	void CreateLookAt()
	{
		m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_camPos, m_camLookAt, DirectX::SimpleMath::Vector3::UnitY);
	}

	


	void SetArcTarget(DirectX::XMFLOAT3 newTarg) { ArcTarget = newTarg;  }
	DirectX::XMFLOAT3 GetArcTarget() { return ArcTarget;  }
private:
	float								m_movespeed;

	DirectX::SimpleMath::Vector3		m_camPos;
	DirectX::SimpleMath::Vector3		m_camOrientation;
	DirectX::SimpleMath::Vector3		m_camLookAt;
	DirectX::SimpleMath::Vector3		m_camLookDirection;
	DirectX::SimpleMath::Vector3		m_camRight;
	float m_camRotRate;

	DirectX::SimpleMath::Matrix			m_view;

	DirectX::SimpleMath::Vector2 oldMouse, newMouse; 

	DirectX::XMFLOAT3 ArcTarget; 

public:
	DirectX::SimpleMath::Matrix GetView()const { return m_view;  }
	DirectX::SimpleMath::Vector3 GetPos() const { return m_camPos;  }
	DirectX::SimpleMath::Vector3 GetRight() const { return m_camRight;  }



	

};




