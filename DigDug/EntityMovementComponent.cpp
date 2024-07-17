#include "EntityMovementComponent.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "Timer.h"
#include "Renderer.h"
#include "InputComponent.h"
#include <SDL_rect.h>
#include "PathwayCreatorComponent.h"
#include <iostream>


void dae::EntityMovementComponent::Update()
{

	m_Rect = GetGameObject()->GetComponent<TextureComponent>()->GetRect();
	SDL_Rect smallerRect{ m_Rect.x, m_Rect.y, m_Rect.w -4, m_Rect.h / 10 };
	SDL_Rect smallerRect2{ m_Rect.x, m_Rect.y, m_Rect.w / 10, m_Rect.h -4 };
	m_LeftRect = smallerRect2;
	m_RightRect = smallerRect2;
	m_BottomRect = smallerRect;
	m_TopRect = smallerRect;

	m_LeftRect.x -= m_LeftRect.w;
	m_LeftRect.y += 2;

	m_RightRect.x += (m_Rect.w);
	m_RightRect.y += 2;

	m_BottomRect.y += (m_Rect.h);
	m_BottomRect.x += 2;

	m_TopRect.y -= m_TopRect.h;
	m_TopRect.x += 2;

	m_Rect.h /= 4;
	m_Rect.w /= 4;
	m_Rect.x += static_cast<int>(m_Rect.w*1.5f);
	m_Rect.y += static_cast<int>(m_Rect.h*1.5f);

	if (!m_CanMove) return;
	if (m_IsAutonomous) {
		//switch (m_State)
		//{
		//case MathLib::EMovingState::MovingUp:
		//	m_PathwayColliderRect = m_TopRect;
		//	break;
		//case MathLib::EMovingState::MovingLeft:
		//	m_PathwayColliderRect = m_LeftRect;
		//	m_Movement = MathLib::LEFT;
		//	break;
		//case MathLib::EMovingState::MovingRight:
		//	m_PathwayColliderRect = m_RightRect;
		//	m_Movement = MathLib::RIGHT;
		//	break;
		//case MathLib::EMovingState::MovingDown:
		//	m_PathwayColliderRect = m_BottomRect;
		//	break;
		//default:
		//	break;
		//}

		if (m_CachedLocation == glm::vec2{ 0,0 }) {
			auto comp{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<PathwayCreatorComponent>() };
			CheckMovement(comp->GetPathways());
		}

		float dx = m_CachedLocation.x - GetGameObject()->GetCenter().x;
		float dy = m_CachedLocation.y - GetGameObject()->GetCenter().y;
		float distanceToTarget = std::sqrt(dx * dx + dy * dy);
		if (distanceToTarget > 1) {
			dx /= distanceToTarget;
			dy /= distanceToTarget;
		}
		else if (distanceToTarget < 1) {
			auto comp{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<PathwayCreatorComponent>() };
			CheckMovement(comp->GetPathways());
		}

		if (dx < 0) {
			m_LastDir = "Left";
		}
		else if (dx > 0) {
			m_LastDir = "Right";
		}

		GetGameObject()->GetTransform()->Translate(dx * 2.f, dy * 2.f);
		GetGameObject()->GetComponent<TextureComponent>()->SetTexture("Enemies/" + m_EnemyName + (m_IsGhostMode ? "Ghost" : m_LastDir) + ".png", 0.2f, 2);
	}
}

void dae::EntityMovementComponent::Render() const
{
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 0, 255); // Set the color to red
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_LeftRect);
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 255, 0, 255); // Set the color to red
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_RightRect);
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 255, 255); // Set the color to red
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_TopRect);	
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 255, 255); // Set the color to red
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_BottomRect);	
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 255, 255); // Set the color to red
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_Rect);	
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_PathwayColliderRect);

	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 255, 255); // Set the color to red
	//auto rrect = SDL_Rect{ int(m_CachedLocation.x), int(m_CachedLocation.y), 1,1 };
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rrect);	
	//auto rrect2 = SDL_Rect{ int(GetGameObject()->GetCenter().x), int(GetGameObject()->GetCenter().y), 5,5 };
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rrect2);


}

void dae::EntityMovementComponent::CheckMovement(const std::map<int, PathWay>& pathways)
{
	
	std::vector<PathWay> paths;
	for (auto item : m_CurrentSurroundingTiles)
	{
		auto path{ pathways.find(item.second) };
		if (path != pathways.end()) {
			if (path->second.PathState == EPathState::Tile) {
					paths.push_back(path->second);
			}
		}
	}

	
		int randIndex{ MathLib::CalculateChance(static_cast<int>(paths.size()) - 1) };

		if (paths.size() > 0) {
			m_CachedLocation = paths[randIndex].Middle;

			if (m_CachedLocation.x > GetGameObject()->GetTransform()->GetWorld().Position.x) {
				m_Movement = MathLib::RIGHT;
				m_LastDir = "Right";
			}
			else {
				m_Movement = MathLib::LEFT;
				m_LastDir = "Left";
			}
		}
}

void dae::EntityMovementComponent::SetMovement(MathLib::Movement movement)
{
	if (!m_CanMove) return;
	m_Movement = movement;
	switch (movement)
	{
	case MathLib::UP:
		m_PathwayColliderRect = m_TopRect;
		break;
	case MathLib::DOWN:
		m_PathwayColliderRect = m_BottomRect;
		break;
	case MathLib::LEFT:
		m_PathwayColliderRect = m_LeftRect;
		break;
	case MathLib::RIGHT:
		m_PathwayColliderRect = m_RightRect;
		break;
	default:
		break;
	}
}

void dae::EntityMovementComponent::SetCurrentTileId(int id)
{
	m_CurrentTileId = id;
	m_CurrentSurroundingTiles = {
		{ MathLib::Movement::DOWN, id + 14},
		{ MathLib::Movement::UP, id - 14},
		{ MathLib::Movement::RIGHT, id + 1},
		{ MathLib::Movement::LEFT, id - 1},
	};
}

SDL_Rect dae::EntityMovementComponent::GetPathCollider(MathLib::Movement movement)
{
	switch (movement)
	{
	case MathLib::UP:
		return m_TopRect;
		break;
	case MathLib::DOWN:
		return m_BottomRect;
		break;
	case MathLib::LEFT:
		return m_LeftRect;
		break;
	case MathLib::RIGHT:
		return m_RightRect;
		break;
	default:
		return SDL_Rect{};
		break;
	}
}
