#include "Timer.h"
#include "InputComponent.h"
#include "GameObject.h"
#include "../DigDug/PathwayCreatorComponent.h"
#include "../DigDug/EntityMovementComponent.h"

void dae::InputComponent::UpdatePos(float dt)
{
	if (!m_CanMove) return;
	if (m_Movespeed.y > 0 || m_Movespeed.y < 0 || m_Movespeed.x > 0 || m_Movespeed.x < 0)
	{
		auto go{ GetGameObject() };

		glm::vec2 currentPosition = go->GetTransform()->GetWorld().Position;
		glm::vec3 futurePosition = glm::vec3(currentPosition.x + (m_Movespeed.x * dt), currentPosition.y + (m_Movespeed.y * dt), 1);

		if (futurePosition.x < 0 || futurePosition.x >(WindowSizeX) - PlayerSize) return;

		if (!m_Movement[MathLib::Side::Top] && m_Movespeed.y < 0) return;
		if (!m_Movement[MathLib::Side::Bottom] && m_Movespeed.y > 0) return;
		if (!m_Movement[MathLib::Side::Left] && m_Movespeed.x < 0) return;
		if (!m_Movement[MathLib::Side::Right] && m_Movespeed.x > 0) return;
		go->GetTransform()->SetPosition(futurePosition.x, futurePosition.y);
		//m_Movespeed = glm::vec3{ 0,0,0 };
	}
}

void dae::InputComponent::Update()
{

	auto dt{ Timer::GetInstance().GetDeltaTime() };

	auto pathCreator{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator]) };
	if (!pathCreator) return;
	auto pathwayComp{ pathCreator->GetComponent<PathwayCreatorComponent>() };
	auto playerComp{ GetGameObject()->GetComponent<EntityMovementComponent>()};

	m_Movement[MathLib::Side::Left] = true;
	m_Movement[MathLib::Side::Right] = true;
	m_Movement[MathLib::Side::Bottom] = true;
	m_Movement[MathLib::Side::Top] = true;

	for (const auto& pathWay : pathwayComp->GetPathways()) {
		const auto rect{ pathWay.second.Rect };
		const auto canMove{ pathWay.second.PathState == MathLib::EPathState::Blocker || pathWay.second.PathState == MathLib::EPathState::Breakable };
		if (MathLib::IsOverlapping(playerComp->GetPathCollider(MathLib::Movement::LEFT), rect) && canMove) {
			m_Movement[MathLib::Side::Left] = false;
		}
		if (MathLib::IsOverlapping(playerComp->GetPathCollider(MathLib::Movement::RIGHT), rect) && canMove) {
			m_Movement[MathLib::Side::Right] = false;
		}
		if (MathLib::IsOverlapping(playerComp->GetPathCollider(MathLib::Movement::DOWN), rect) && canMove) {
			m_Movement[MathLib::Side::Bottom] = false;
		}
		if (MathLib::IsOverlapping(playerComp->GetPathCollider(MathLib::Movement::UP), rect) && canMove) {
			m_Movement[MathLib::Side::Top] = false;
		}

	}
	/*for (const auto& pathWay : pathwayComp->GetHorizontalPathways()) {
		if (MathLib::IsOverlapping(playerComp->GetPathCollider(MathLib::Movement::LEFT), pathWay)) {
			m_Movement[MathLib::Side::Left] = true;
		}
		if (MathLib::IsOverlapping(playerComp->GetPathCollider(MathLib::Movement::RIGHT), pathWay)) {
			m_Movement[MathLib::Side::Right] = true;
		}
	}
	for (const auto& pathWay : pathwayComp->GetVerticalPathways()) {
		if (MathLib::IsOverlapping(playerComp->GetPathCollider(MathLib::Movement::DOWN), pathWay)) {
			m_Movement[MathLib::Side::Bottom] = true;
		}
		if (MathLib::IsOverlapping(playerComp->GetPathCollider(MathLib::Movement::UP), pathWay)) {
			m_Movement[MathLib::Side::Top] = true;
		}
	}*/

	UpdatePos(dt);

	if (m_IsController) {
		StopMovement(MathLib::LEFT);
		StopMovement(MathLib::RIGHT);
		StopMovement(MathLib::UP);
		StopMovement(MathLib::DOWN);
	}
}

void dae::InputComponent::SetMoveSpeed(const glm::vec3& movespeed)
{
	m_Movespeed = movespeed;
}

void dae::InputComponent::SetMoveSpeed(const glm::vec3& movespeed, MathLib::Movement direction, bool isController)
{
	m_MoveSpeedList.insert(std::pair<MathLib::Movement, const glm::vec3>{ direction, movespeed });
	m_Movespeed = movespeed;
	m_IsController = isController;
}

void dae::InputComponent::StopMovement(MathLib::Movement direction)
{
	m_MoveSpeedList.erase(direction);
	m_Movespeed = m_MoveSpeedList.size() > 0 ? m_MoveSpeedList.begin()->second : glm::vec3{ 0,0,0 };
}
