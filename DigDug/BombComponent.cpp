#include "BombComponent.h"
#include "EntityMovementComponent.h"
#include "EnemyComponent.h"
#include "Timer.h"
#include "InputComponent.h"
#include "Renderer.h"
#include <AudioComponent.h>
#include <iostream>

void dae::BombComponent::Init()
{
}

void dae::BombComponent::StartBomb(glm::vec3 /*loc*/, int tileId)
{
	auto comp{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<PathwayCreatorComponent>() };
	auto& pathways{ comp->GetPathways() };

	m_TileId = tileId;
	const auto path{ pathways.at(tileId) };
	glm::vec2 pos2{ path.Rect.x - path.Rect.w, path.Rect.y };
	GetGameObject()->GetTransform()->SetPosition(pos2);
	GetGameObject()->AddComponent(std::make_unique<dae::AudioComponent>())->PlayBombSound();
	GetGameObject()->AddComponent(std::make_unique<TextureComponent>())->SetTexture("Character/bomb.png", 0.2f, 3);
}

void dae::BombComponent::HandleExplosionPlacement(int& index, const std::map<int, dae::PathWay>& pathways, bool &outHitWall)
{
	if (pathways.find(index) == pathways.end()) return;
	if (index >= 0 && index < GridSize*GridSize) {
		if (pathways.at(index).PathState == EPathState::Tile && !outHitWall) {
			auto comp{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<PathwayCreatorComponent>() };
			comp->ActivateBomb(index);

			glm::vec2 pos{ pathways.at(index).Rect.x - pathways.at(index).Rect.w, pathways.at(index).Rect.y };
			pathways.at(index).TextureComponent->SetPosition(pos.x, pos.y);
			pathways.at(index).TextureComponent->SetIsVisible(true);
			pathways.at(index).TextureComponent->SetTexture("Character/explosion.png", 0.3f, 4);

		}
		else {
			outHitWall = true;
		}
	}
}

void dae::BombComponent::Update()
{
	// TODO Animate fuse sequence 
	// TODO make state
	m_FuseTimer -= Timer::GetInstance().GetDeltaTime();
	if (m_FuseTimer <= 0) {
		// TODO trigger explosion & animation sequence
		auto comp{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<PathwayCreatorComponent>() };
		auto& pathways{ comp->GetPathways() };

		const auto path{ pathways.at(m_TileId) };
		glm::vec2 pos2{ path.Rect.x - path.Rect.w, path.Rect.y };
		for (int i = 1; i <= m_BombStrength; i++) {

			//int row = m_TileId / GridSize;
			//int col = m_TileId % GridSize;

			int leftIndex = (m_TileId - i);
			int rightIndex = (m_TileId + i);
			int topIndex = (m_TileId - (GridSize * i));
			int bottomIndex = (m_TileId + (GridSize * i));

			// Check left index (same row)
			HandleExplosionPlacement(leftIndex, pathways, m_HitWallLeft);
			HandleExplosionPlacement(rightIndex, pathways, m_HitWallRight);
			HandleExplosionPlacement(topIndex, pathways, m_HitWallTop);
			HandleExplosionPlacement(bottomIndex, pathways, m_HitWallBottom);
		}
		m_FuseTimer = 9999;
	}

	m_ExplodeTimer -= Timer::GetInstance().GetDeltaTime();
	if (m_ExplodeTimer <= 0) {
		for (int i = 1; i <= m_BombStrength; i++) {
			int leftIndex = (m_TileId - i);
			int rightIndex = (m_TileId + i);
			int topIndex = (m_TileId - (GridSize * i));
			int bottomIndex = (m_TileId + (GridSize * i));
			HandleExplosionEnd(leftIndex);
			HandleExplosionEnd(rightIndex);
			HandleExplosionEnd(topIndex);
			HandleExplosionEnd(bottomIndex);
		}
		m_ExplodeTimer = 9999;
		GetGameObject()->MarkForDestroy();
	}
}

void dae::BombComponent::HandleExplosionEnd(int& index)
{
	if (index >= 0 && index < GridSize * GridSize) {
		auto comp{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<PathwayCreatorComponent>() };
		comp->ActivatePathway(index);
	}
}

void dae::BombComponent::Render() const
{
	//auto texComp = GetGameObject()->GetComponent<TextureComponent>();
	//auto texComp2 = m_Player->GetComponent<TextureComponent>()->GetRect();
	//auto rect = texComp->GetRect();
	//rect.w /= 3;
	//rect.h /= 3;
	//rect.x = texComp2.x + texComp2.w / 2;
	//rect.y = texComp2.y + texComp2.h / 2;

	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255); // Set the color to red
	//SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &rect); // D
	//auto comp{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<PathwayCreatorComponent>() };
	//auto pathways{ comp->GetPathways() };
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255); // Set the color to red
	//std::cout << m_TileId << std::endl;
	//for (int i = 0; i <= m_BombStrength; i++)
	//{
	//	if (i == 0) continue;
	//	//int row = m_TileId / GridSize;
	//	//int col = m_TileId % GridSize;

	//	int leftIndex = m_TileId - i;
	//	int rightIndex = m_TileId + i;
	//	int topIndex = m_TileId - (GridSize * i);
	//	int bottomIndex = m_TileId + (GridSize * i);
	//	if (leftIndex > 0 && leftIndex < GridSize * GridSize && pathways.at(leftIndex).PathState == EPathState::Tile) {
	//		auto rect{ pathways.at(leftIndex).Rect };
	//		SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
	//	}
	//	if (rightIndex > 0 && rightIndex < GridSize * GridSize && pathways.at(rightIndex).PathState == EPathState::Tile) {
	//		auto rect{ pathways.at(rightIndex).Rect };
	//		SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
	//	}
	//	if (topIndex > 0 && topIndex < GridSize * GridSize && pathways.at(topIndex).PathState == EPathState::Tile) {
	//		SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &pathways.at(topIndex).Rect);
	//	}
	//	if (bottomIndex > 0 && bottomIndex < GridSize * GridSize && pathways.at(bottomIndex).PathState == EPathState::Tile) {
	//		SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &pathways.at(bottomIndex).Rect);
	//	}
	//}
}