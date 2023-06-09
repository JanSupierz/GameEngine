#include "CameraComponent.h"
#include "SceneManager.h"
#include "GameObject.h"
#include <numeric>
#include "Renderer.h"
#include "CameraTargetComponent.h"

dae::CameraComponent::CameraComponent(float ignoreDistance, int priority)
	:Component(priority), m_IgnoreDistance{ ignoreDistance }
{
}

dae::CameraComponent::~CameraComponent()
{
	for (GameObject* pObject : m_pObjectsToFollow)
	{
		pObject->GetComponent<CameraTargetComponent>()->SetCamera(nullptr);
	}
}

void dae::CameraComponent::Update()
{
	GameObject* pCamera{ GetOwner() };

	//Save old position
	const glm::vec2 oldPos{ pCamera->GetWorldPosition() };

	//Get average position
	glm::vec2 averagePos{};

	for (GameObject* pObject : m_pObjectsToFollow)
	{
		averagePos += pObject->GetWorldPosition();
	}

	const int nrObjects{ static_cast<int>(m_pObjectsToFollow.size())};
	if (nrObjects > 0)
	{
		averagePos /= nrObjects;
	}

	//Clamp between the bounds
	averagePos.x -= m_IgnoreDistance;
	averagePos.y -= m_IgnoreDistance;

	averagePos = glm::clamp(averagePos, m_BoundsMin, m_BoundsMax);

	pCamera->SetPosition(averagePos);
}

void dae::CameraComponent::SetIgnoreDistance(float distance)
{
	m_IgnoreDistance = distance;
}

void dae::CameraComponent::SetBounds(const glm::vec2& min, const glm::vec2& max)
{
	m_BoundsMin = min;

	//Apply offsets
	m_BoundsMax = max - Renderer::GetInstance().GetWindowSize();

	//Max value has to be at least the min value
	m_BoundsMax = glm::max(m_BoundsMin, m_BoundsMax);
}

void dae::CameraComponent::AddObjectToFollow(GameObject* pObject)
{
	m_pObjectsToFollow.push_back(pObject);
}

void dae::CameraComponent::RemoveObjectToFollow(GameObject* pObject)
{
	m_pObjectsToFollow.erase(std::remove(m_pObjectsToFollow.begin(), m_pObjectsToFollow.end(), pObject), m_pObjectsToFollow.end());
}
