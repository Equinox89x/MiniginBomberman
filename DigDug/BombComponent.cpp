#include "BombComponent.h"
#include "EntityMovementComponent.h"
#include "EnemyComponent.h"
#include "Timer.h"
#include "InputComponent.h"
#include "Renderer.h"
#include <AudioComponent.h>
#include <iostream>

void dae::BombComponent::Update()
{
	auto comp{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<PathwayCreatorComponent>() };
	auto pathways{ comp->GetPathways() };

	auto bombTexComp{ GetGameObject()->GetComponent<TextureComponent>() };
	if (MathLib::IsOverlapping(pathways.at(m_TileId).TextureComponent->GetRect(), bombTexComp->GetRect())) {
		if (pathways.at(m_TileId).PathStats.PathState == MathLib::EPathState::Explosion && GetState() == MathLib::EBombState::Fuse) {
			SetState(new ExplosionState(m_Scene), MathLib::EBombState::Explosion);
		}
	}

	m_BombState->Update(GetGameObject());
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

void dae::BombComponent::Init()
{
}

void dae::BombComponent::StartBomb(int tileId, int bombStrength)
{
	m_TileId = tileId;
	m_BombStrength = bombStrength;
	SetState(new FuseState(m_Scene), MathLib::EBombState::Fuse);

}
