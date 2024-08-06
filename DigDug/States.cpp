#include "States.h"
#include "BombComponent.h"
#include "EnemyComponent.h"
#include "EntityMovementComponent.h"
// #include "FloatingScoreComponent.h"
#include "PathwayCreatorComponent.h"
#include "PlayerComponent.h"
#include "ResourceManager.h"
#include <AudioComponent.h>
#include <FPSCounterComponent.h>
#include <InputComponent.h>
#include <ValuesComponent.h>

#pragma region Bomb

void dae::FuseState::OnStart(GameObject* gameObject)
{
	auto  bombComp{ gameObject->GetComponent<BombComponent>() };
	auto  comp{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<PathwayCreatorComponent>() };
	auto& pathways{ comp->GetPathways() };
	auto& path{ pathways.at(bombComp->GetTileId()) };
	path.BombDropper = bombComp->BombDropper;

	glm::vec2 pos2{ path.Rect.x, path.Rect.y };
	gameObject->GetTransform()->SetPosition(pos2);
	gameObject->AddComponent(std::make_unique<dae::AudioComponent>())->PlayBombSound();
	gameObject->AddComponent(std::make_unique<TextureComponent>())->SetTexture("Character/bomb.png", 0.2f, 3);
}

void dae::FuseState::Update(GameObject* gameObject)
{
	if (auto bombComp{ gameObject->GetComponent<BombComponent>() })
	{
		m_FuseTimer -= Timer::GetInstance().GetDeltaTime();
		if (m_FuseTimer <= 0)
		{
			bombComp->SetState(new ExplosionState(m_Scene), MathLib::EBombState::Explosion);
		}
	}
}

void dae::ExplosionState::Update(GameObject* gameObject)
{
	m_ExplodeTimer -= Timer::GetInstance().GetDeltaTime();
	if (m_ExplodeTimer <= 0)
	{
		for (int i = 1; i <= m_BombStrength; i++)
		{
			int leftIndex = (m_TileId - i);
			int rightIndex = (m_TileId + i);
			int topIndex = (m_TileId - (GridSize * i));
			int bottomIndex = (m_TileId + (GridSize * i));
			HandleExplosionEnd(leftIndex);
			HandleExplosionEnd(rightIndex);
			HandleExplosionEnd(topIndex);
			HandleExplosionEnd(bottomIndex);
		}
		HandleExplosionEnd(m_TileId);
		gameObject->GetComponent<BombComponent>()->SetState(new DeathState(m_Scene), MathLib::EBombState::Death);
	}
}

void dae::ExplosionState::OnStart(GameObject* gameObject)
{
	gameObject->GetComponent<AudioComponent>()->PlayExplosionSound();
	m_TileId = gameObject->GetComponent<BombComponent>()->GetTileId();
	m_BombStrength = gameObject->GetComponent<BombComponent>()->GetBombStrength();
	// if (!m_Scene && m_Ptr != reinterpret_cast<int*>(m_Scene)) return;
	auto* comp{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<PathwayCreatorComponent>() };
	auto& pathways{ comp->GetPathways() };

	for (int i = 1; i <= m_BombStrength; i++)
	{
		int leftIndex = (m_TileId - i);
		int rightIndex = (m_TileId + i);
		int topIndex = (m_TileId - (GridSize * i));
		int bottomIndex = (m_TileId + (GridSize * i));

		// Check left index (same row)
		HandleExplosionPlacement(leftIndex, pathways, m_HitWallLeft, 90.f);
		HandleExplosionPlacement(rightIndex, pathways, m_HitWallRight, 90.f);
		HandleExplosionPlacement(topIndex, pathways, m_HitWallTop);
		HandleExplosionPlacement(bottomIndex, pathways, m_HitWallBottom);
	}

	glm::vec2 pos{ pathways.at(m_TileId).Rect.x - pathways.at(m_TileId).Rect.w, pathways.at(m_TileId).Rect.y };
	//pathways.at(m_TileId).PathObject->GetComponent<PathObject>()->SetPosition(pos.x, pos.y);
	pathways.at(m_TileId).PathObject->GetComponent<TextureComponent>()->SetTexture("Character/explosionCenter.png", 0.125f, 4);
	gameObject->GetComponent<TextureComponent>()->SetIsVisible(false);
	comp->ActivateBomb(m_TileId);
}

void dae::ExplosionState::HandleExplosionPlacement(int& index, const std::map<int, dae::PathWay>& pathways, bool& outHitWall, float /*rotationOffset*/)
{
	if (pathways.find(index) == pathways.end())
		return;
	if (index >= 0 && index < GridSize * GridSize)
	{
		if (pathways.at(index).PathStats.PathType == MathLib::EPathType::Tile && !outHitWall)
		{
			auto comp{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<PathwayCreatorComponent>() };
			comp->ActivateBomb(index);

			glm::vec2 pos{ pathways.at(index).Rect.x - pathways.at(index).Rect.w, pathways.at(index).Rect.y };
			pathways.at(index).BombDropper = pathways.at(m_TileId).BombDropper;
			//pathways.at(index).PathObject->SetPosition(pos.x, pos.y);
			//pathways.at(index).PathObject->GetComponent<TextureComponent>()->SetIsVisible(true);
			//pathways.at(index).PathObject->GetComponent<TextureComponent>()->Rotate(rotationOffset);
			pathways.at(index).PathObject->GetComponent<TextureComponent>()->SetTexture("Character/explosion.png", 0.125f, 4);
		}
		else
		{
			outHitWall = true;
		}
	}
}

void dae::ExplosionState::HandleExplosionEnd(int& index)
{
	if (index >= 0 && index < GridSize * GridSize)
	{
		auto comp{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<PathwayCreatorComponent>() };
		comp->ActivatePathway(index);
	}
}
#pragma endregion

#pragma region Characters

void dae::BombedState::OnStart(GameObject* pGameObject)
{
	if (auto enemyComp{ pGameObject->GetComponent<EnemyComponent>() })
	{
		pGameObject->GetComponent<EntityMovementComponent>()->DisableMovement(true);

		auto		font{ ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 10) };
		auto		pos{ pGameObject->GetTransform()->GetWorld().Position };
		auto		stats{ enemyComp->GetEnemyStats() };
		std::string deathTextureName{ stats.Name + "Bombed.png" };
		pGameObject->GetComponent<TextureComponent>()->SetTexture("Enemies/" + deathTextureName);

		if (auto player{ enemyComp->GetPlayer() })
		{
			player->GetComponent<ValuesComponent>()->IncreaseScore(stats.Points);
		}
	}

	if (auto playerComp{ pGameObject->GetComponent<PlayerComponent>() })
	{
		pGameObject->GetComponent<TextureComponent>()->SetTexture("Character/PlayerDeath.png", 0.2f, 6);
		pGameObject->GetComponent<InputComponent>()->DisableMovement(true);
		pGameObject->GetComponent<EntityMovementComponent>()->DisableMovement(true);
		pGameObject->GetComponent<ValuesComponent>()->Damage();
	}
}

void dae::BombedState::Update(GameObject* pGameObject)
{
	m_DeathTimer -= Timer::GetInstance().GetDeltaTime();
	if (m_DeathTimer <= 0)
	{
		if (auto enemyComp{ pGameObject->GetComponent<EnemyComponent>() })
		{
			enemyComp->SetState(new DeathState(m_Scene), MathLib::ELifeState::DEAD);
		}
		else if (auto playerComp{ pGameObject->GetComponent<PlayerComponent>() })
		{
			if (pGameObject->GetComponent<ValuesComponent>()->GetLives() > 0)
			{
				playerComp->SetState(new AliveState(m_Scene), MathLib::ELifeState::ALIVE);
			}
			else
			{
				playerComp->SetState(new DeathState(m_Scene), MathLib::ELifeState::DEAD);
			}
		}
	}
}

void dae::AliveState::OnStart(GameObject* pGameObject)
{
	if (!pGameObject)
		return;
	if (auto enemyComp{ pGameObject->GetComponent<EnemyComponent>() })
	{
	}
	else if (auto playerComp{ pGameObject->GetComponent<PlayerComponent>() })
	{
		pGameObject->GetComponent<TextureComponent>()->SetTexture("Character/moveDown.png", 0.2f, 3);
		playerComp->Reposition();
		pGameObject->GetComponent<InputComponent>()->DisableMovement(false);
		pGameObject->GetComponent<EntityMovementComponent>()->DisableMovement(false);
	}
}

void dae::AliveState::Update(GameObject* pGameObject)
{
	if (m_Scene && pGameObject)
	{
		if (pGameObject->IsMarkedForDestroy())
			return;
		if (auto playerComp{ pGameObject->GetComponent<PlayerComponent>() })
		{
			auto enemies{ m_Scene->GetGameObjects(EnumStrings[Names::EnemyGeneral], false) };
			for (const auto& enemy : enemies)
			{
				if (enemy && !enemy->IsMarkedForDestroy())
				{
					auto lifestate{ enemy->GetComponent<EnemyComponent>()->GetState() };
					if (lifestate != MathLib::ELifeState::BOMBED && lifestate != MathLib::ELifeState::DEAD)
					{
						if (pGameObject && !pGameObject->IsMarkedForDestroy())
						{
							if (MathLib::IsOverlapping(pGameObject->GetComponent<TextureComponent>()->GetRect(), enemy->GetComponent<TextureComponent>()->GetRect()))
							{
								pGameObject->GetComponent<PlayerComponent>()->SetState(new BombedState(m_Scene), MathLib::ELifeState::BOMBED);
							}
						}
					}
				}
			}
		}
	}
}

void dae::DeathState::OnStart(GameObject* pGameObject)
{
	if (pGameObject->GetComponent<PlayerComponent>())
	{
		pGameObject->MarkForDestroy();
		m_hasDeathSequence = false;
	}
	else if (auto enemyComp{ pGameObject->GetComponent<EnemyComponent>() })
	{
		pGameObject->GetComponent<TextureComponent>()->SetTexture("Enemies/EnemyDeath.png", 0.2f, 4);
		auto children{ m_Scene->GetGameObjects(EnumStrings[Names::EnemyGeneral], false) };
		if (children.size() <= 1)
		{
			m_Scene->GetGameObject(EnumStrings[Names::Global])->GetComponent<AudioComponent>()->QueueMusic(FindDoorMusic, true);
		}
	}
	else
	{
		m_DeathTimer = 0.1f;
	}
};

void dae::DeathState::Update(GameObject* pGameObject)
{
	m_DeathTimer -= Timer::GetInstance().GetDeltaTime();
	if (m_DeathTimer < 0)
	{
		pGameObject->MarkForDestroy();
		m_DeathTimer = 999;
	}
}
#pragma endregion
