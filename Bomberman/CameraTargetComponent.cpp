#include "CameraTargetComponent.h"
#include "CameraComponent.h"

dae::CameraTargetComponent::CameraTargetComponent(int priority)
	:Component(priority), m_pCamera{ nullptr }
{
}

dae::CameraTargetComponent::~CameraTargetComponent()
{
	if (m_pCamera)
	{
		m_pCamera->RemoveObjectToFollow(GetOwner());
	}
}

void dae::CameraTargetComponent::SetCamera(CameraComponent* pCamera)
{
	//Remove from the old one
	if (m_pCamera)
	{
		m_pCamera->RemoveObjectToFollow(GetOwner());
	}

	//Swap
	m_pCamera = pCamera;


	//Add to the new one
	if (m_pCamera)
	{
		m_pCamera->AddObjectToFollow(GetOwner());
	}
}