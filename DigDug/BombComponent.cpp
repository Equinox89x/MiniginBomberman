#include "BombComponent.h"
#include "EnemyComponent.h"
#include "EntityMovementComponent.h"
#include "InputComponent.h"
#include "Renderer.h"
#include "Timer.h"
#include <AudioComponent.h>
#include <iostream>

void dae::BombComponent::Update()
{
	auto comp{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<PathwayCreatorComponent>() };
	auto pathways{ comp->GetPathways() };

	auto bombTexComp{ GetGameObject()->GetComponent<TextureComponent>() };
	if (MathLib::IsOverlapping(pathways.at(m_TileId).TextureComponent->GetRect(), bombTexComp->GetRect()))
	{
		if (pathways.at(m_TileId).PathStats.PathState == MathLib::EPathState::Explosion && GetState() == MathLib::EBombState::Fuse)
		{
			SetState(new ExplosionState(m_Scene), MathLib::EBombState::Explosion);
		}
	}

	m_BombState->Update(GetGameObject());
}

void dae::BombComponent::Render() const {}

void dae::BombComponent::Init() {}

void dae::BombComponent::StartBomb(int tileId, int bombStrength)
{
	m_TileId = tileId;
	m_BombStrength = bombStrength;
	SetState(new FuseState(m_Scene), MathLib::EBombState::Fuse);
}
