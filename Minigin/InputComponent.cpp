#include "InputComponent.h"
#include "../Bomberman/EntityMovementComponent.h"
#include "../Bomberman/PathwayCreatorComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Timer.h"

void dae::InputComponent::UpdatePos(float dt)
{
	if (!m_CanMove)
		return;
	if (m_Movespeed.y > 0 || m_Movespeed.y < 0 || m_Movespeed.x > 0 || m_Movespeed.x < 0)
	{
		const glm::vec2 currentPosition = GetGameObject()->GetTransform()->GetWorld().Position;
		const glm::vec3 futurePosition = glm::vec3(currentPosition.x + (m_Movespeed.x * dt), currentPosition.y + (m_Movespeed.y * dt), 1);

		if (futurePosition.x < 0 || futurePosition.x > (WindowSizeX)-PlayerSize)
			return;

		if (!m_Movement[MathLib::ESide::Top] && m_Movespeed.y < 0)
			return;
		if (!m_Movement[MathLib::ESide::Bottom] && m_Movespeed.y > 0)
			return;
		if (!m_Movement[MathLib::ESide::Left] && m_Movespeed.x < 0)
			return;
		if (!m_Movement[MathLib::ESide::Right] && m_Movespeed.x > 0)
			return;
		GetGameObject()->GetTransform()->SetPosition(futurePosition.x, futurePosition.y);
		// m_Movespeed = glm::vec3{ 0,0,0 };

		MoveCameraOverlapper(dt);
	}
}

void dae::InputComponent::MoveCameraOverlapper(float dt)
{
	if (!m_HasOverlaps)
		return;
	if (MathLib::IsOverlapping(m_MoveOverlap, GetGameObject()->GetComponent<TextureComponent>()->GetRect()))
	{
		const auto& gameObjects{ m_Scene->GetGameObjects() };
		if (m_Movement[MathLib::ESide::Left])
		{
			m_Offset += -m_Movespeed.x * dt;
			for (const auto& obj : gameObjects)
			{
				if (obj->GetName() != EnumStrings[Names::Global])
				{
					obj->GetTransform()->Translate({ -m_Movespeed.x * dt, 0, 0 });
				}
			}

			m_MoveOverlap.x -= static_cast<int>(m_Movespeed.x * dt);
		}
		else if (m_Movement[MathLib::ESide::Right])
		{
			m_Offset += -m_Movespeed.x * dt;
			for (const auto& obj : gameObjects)
			{
				if (obj->GetName() != EnumStrings[Names::Global])
				{
					obj->GetTransform()->Translate({ -m_Movespeed.x * dt, 0, 0 });
				}
			}

			m_MoveOverlap.x -= static_cast<int>(m_Movespeed.x * dt);
		}
	}
}

void dae::InputComponent::ResetOverlap(glm::vec2 originalPos) {
	if (!m_HasOverlaps)
		return;
	const auto distance = GetGameObject()->GetTransform()->GetWorld().Position - glm::vec3{ originalPos.x, originalPos.y, 0 };

	const auto& gameObjects{ m_Scene->GetGameObjects() };
	for (const auto& obj : gameObjects)
	{
		if (obj->GetName() != EnumStrings[Names::Global])
		{
			obj->GetTransform()->Translate({ -m_Offset, 0, 0 });
		}
	}

	m_MoveOverlap = SDL_Rect{ Cellsize * 9, 0, Cellsize * 7, WindowSizeY };
	m_Offset = 0;
}

void dae::InputComponent::Update()
{
	const auto dt{ Timer::GetInstance().GetDeltaTime() };

	auto pathCreator{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator]) };
	if (!pathCreator)
		return;
	const auto pathwayComp{ pathCreator->GetComponent<PathwayCreatorComponent>() };
	const auto playerComp{ GetGameObject()->GetComponent<EntityMovementComponent>() };

	m_Movement[MathLib::ESide::Left] = true;
	m_Movement[MathLib::ESide::Right] = true;
	m_Movement[MathLib::ESide::Bottom] = true;
	m_Movement[MathLib::ESide::Top] = true;

	for (const auto& pathWay : pathwayComp->GetPathways())
	{
		const auto rect{ pathWay.second.Rect };
		const auto canMove{ pathWay.second.PathStats.PathType != MathLib::EPathType::Tile };
		if (MathLib::IsOverlapping(playerComp->GetPathCollider(MathLib::EMovement::LEFT), rect) && canMove)
		{
			m_Movement[MathLib::ESide::Left] = false;
		}
		if (MathLib::IsOverlapping(playerComp->GetPathCollider(MathLib::EMovement::RIGHT), rect) && canMove)
		{
			m_Movement[MathLib::ESide::Right] = false;
		}
		if (MathLib::IsOverlapping(playerComp->GetPathCollider(MathLib::EMovement::DOWN), rect) && canMove)
		{
			m_Movement[MathLib::ESide::Bottom] = false;
		}
		if (MathLib::IsOverlapping(playerComp->GetPathCollider(MathLib::EMovement::UP), rect) && canMove)
		{
			m_Movement[MathLib::ESide::Top] = false;
		}
	}
	/*for (const auto& pathWay : pathwayComp->GetHorizontalPathways()) {
		if (MathLib::IsOverlapping(playerComp->GetPathCollider(MathLib::EMovement::LEFT), pathWay)) {
			m_Movement[MathLib::ESide::Left] = true;
		}
		if (MathLib::IsOverlapping(playerComp->GetPathCollider(MathLib::EMovement::RIGHT), pathWay)) {
			m_Movement[MathLib::ESide::Right] = true;
		}
	}
	for (const auto& pathWay : pathwayComp->GetVerticalPathways()) {
		if (MathLib::IsOverlapping(playerComp->GetPathCollider(MathLib::EMovement::DOWN), pathWay)) {
			m_Movement[MathLib::ESide::Bottom] = true;
		}
		if (MathLib::IsOverlapping(playerComp->GetPathCollider(MathLib::EMovement::UP), pathWay)) {
			m_Movement[MathLib::ESide::Top] = true;
		}
	}*/

	UpdatePos(dt);

	if (m_IsController)
	{
		StopMovement(MathLib::EMovement::LEFT);
		StopMovement(MathLib::EMovement::RIGHT);
		StopMovement(MathLib::EMovement::UP);
		StopMovement(MathLib::EMovement::DOWN);
	}
}

//void dae::InputComponent::Render() const
//{
//	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 0, 255, 200);
//
//		SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &m_MoveOverlap);
//	
//}

void dae::InputComponent::SetMoveSpeed(const glm::vec3& movespeed) { m_Movespeed = movespeed; }

void dae::InputComponent::SetMoveSpeed(const glm::vec3& movespeed, MathLib::EMovement direction, bool isController)
{
	m_MoveSpeedList.insert(std::pair<MathLib::EMovement, const glm::vec3>{ direction, movespeed });
	m_Movespeed = movespeed;
	m_IsController = isController;
}

void dae::InputComponent::StopMovement(MathLib::EMovement direction)
{
	m_MoveSpeedList.erase(direction);
	m_Movespeed = m_MoveSpeedList.size() > 0 ? m_MoveSpeedList.begin()->second : glm::vec3{ 0, 0, 0 };
}
