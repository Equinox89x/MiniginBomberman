#include "EntityMovementComponent.h"
#include "GameObject.h"
#include "InputComponent.h"
#include "PathwayCreatorComponent.h"
#include "Renderer.h"
#include "TextureComponent.h"
#include "Timer.h"
#include <SDL_rect.h>
#include <iostream>

void dae::EntityMovementComponent::Update()
{

	m_Rect = GetGameObject()->GetComponent<TextureComponent>()->GetRect();
	SDL_Rect smallerRect{ m_Rect.x, m_Rect.y, m_Rect.w - 4, m_Rect.h / 10 };
	SDL_Rect smallerRect2{ m_Rect.x, m_Rect.y, m_Rect.w / 10, m_Rect.h - 4 };
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
	m_Rect.x += static_cast<int>(m_Rect.w * 1.5f);
	m_Rect.y += static_cast<int>(m_Rect.h * 1.5f);

	if (!m_CanMove)
		return;
	if (m_IsAutonomous)
	{
		auto comp{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<PathwayCreatorComponent>() };
		auto& path = comp->GetPathways().find(m_PathId)->second;
		if (path.Middle == glm::vec2{ 0, 0 })
		{
			//CheckMovement(comp->GetPathways());
		}

		float dx = path.Middle.x - GetGameObject()->GetCenter().x;
		float dy = path.Middle.y - GetGameObject()->GetCenter().y;
		float distanceToTarget = std::sqrt(dx * dx + dy * dy);
		if (distanceToTarget > 1)
		{
			dx /= distanceToTarget;
			dy /= distanceToTarget;
		}
		else if (distanceToTarget < 1)
		{
			CheckMovement(comp->GetPathways());
		}

		if (dx < 0)
		{
			m_LastDir = "Left";
		}
		else if (dx > 0)
		{
			m_LastDir = "Right";
		}

		GetGameObject()->GetTransform()->Translate(dx * 1.5f, dy * 1.5f);
		GetGameObject()->GetComponent<TextureComponent>()->SetTexture("Enemies/" + m_EnemyName + /*m_LastDir +*/ ".png", 0.2f, 2);
	}
}

void dae::EntityMovementComponent::Render() const
{
	// SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 0, 255); // Set the color to red
	// SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_LeftRect);
	// SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 255, 0, 255); // Set the color to red
	// SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_RightRect);
	// SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 255, 255); // Set the color to red
	// SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_TopRect);
	// SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 255, 255); // Set the color to red
	// SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_BottomRect);
	// SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 255, 255); // Set the color to red
	// SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_Rect);
	// SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_PathwayColliderRect);

	// auto comp{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<PathwayCreatorComponent>() };
	// auto pathways{ comp->GetPathways() };
	// auto rrect2 = pathways.at(m_CurrentTileId).PathObject->GetRect();
	// SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 255, 255); // Set the color to red
	////auto rrect = SDL_Rect{ int(m_CachedLocation.x), int(m_CachedLocation.y), 1,1 };
	////SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rrect);
	// SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rrect2);
}

void dae::EntityMovementComponent::CheckMovement(const std::map<int, PathWay>& pathways)
{

	std::vector<int> paths;
	for (auto& item : m_CurrentSurroundingTiles)
	{
		const auto& path{ pathways.find(item.second) };
		if (path != pathways.end())
		{
			if (path->second.PathStats.PathType == MathLib::EPathType::Tile)
			{
				paths.push_back(path->second.id);
			}
		}
	}

	if (paths.size() > 0)
	{
		int randIndex{ MathLib::CalculateChance(static_cast<int>(paths.size()) - 1) };
		m_PathId = paths[randIndex];
		if (pathways.find(m_PathId)->second.Middle.x > GetGameObject()->GetTransform()->GetWorld().Position.x)
		{
			m_Movement = MathLib::EMovement::RIGHT;
			m_LastDir = "Right";
		}
		else
		{
			m_Movement = MathLib::EMovement::LEFT;
			m_LastDir = "Left";
		}
	}
}

void dae::EntityMovementComponent::SetMovement(MathLib::EMovement movement)
{
	if (!m_CanMove)
		return;
	m_Movement = movement;
	switch (movement)
	{
	case MathLib::EMovement::UP:
		m_PathwayColliderRect = m_TopRect;
		break;
	case MathLib::EMovement::DOWN:
		m_PathwayColliderRect = m_BottomRect;
		break;
	case MathLib::EMovement::LEFT:
		m_PathwayColliderRect = m_LeftRect;
		break;
	case MathLib::EMovement::RIGHT:
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
		{ MathLib::EMovement::DOWN, id + GridSize },
		{ MathLib::EMovement::UP, id - GridSize },
		{ MathLib::EMovement::RIGHT, id + 1 },
		{ MathLib::EMovement::LEFT, id - 1 },
	};
}

SDL_Rect dae::EntityMovementComponent::GetPathCollider(MathLib::EMovement movement)
{
	switch (movement)
	{
	case MathLib::EMovement::UP:
		return m_TopRect;
		break;
	case MathLib::EMovement::DOWN:
		return m_BottomRect;
		break;
	case MathLib::EMovement::LEFT:
		return m_LeftRect;
		break;
	case MathLib::EMovement::RIGHT:
		return m_RightRect;
		break;
	default:
		return SDL_Rect{};
		break;
	}
}
