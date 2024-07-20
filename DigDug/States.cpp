#include "States.h"
#include "PlayerComponent.h"
#include "PathwayCreatorComponent.h"
#include "BombComponent.h"
#include <AudioComponent.h>
#include "EnemyComponent.h"
#include <FPSCounterComponent.h>
#include "FloatingScoreComponent.h"
#include "ResourceManager.h"
#include "EntityMovementComponent.h"
#include <ValuesComponent.h>
#include <InputComponent.h>

void dae::FuseState::OnStart(GameObject* gameObject)
{
	auto tileId{ gameObject->GetComponent<BombComponent>()->GetTileId() };
	auto comp{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<PathwayCreatorComponent>() };
	auto& pathways{ comp->GetPathways() };
	const auto path{ pathways.at(tileId) };

	glm::vec2 pos2{ path.Rect.x - path.Rect.w, path.Rect.y };
	gameObject->GetTransform()->SetPosition(pos2);
	gameObject->AddComponent(std::make_unique<dae::AudioComponent>())->PlayBombSound();
	gameObject->AddComponent(std::make_unique<TextureComponent>())->SetTexture("Character/bomb.png", 0.2f, 3);

}

void dae::FuseState::Update(GameObject* gameObject)
{
	m_FuseTimer -= Timer::GetInstance().GetDeltaTime();
	if (m_FuseTimer <= 0) {
		
		gameObject->GetComponent<BombComponent>()->SetState(new ExplosionState(m_Scene), MathLib::EBombState::Explosion);
	}
}

void dae::ExplosionState::Update(GameObject* gameObject)
{
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
		gameObject->GetComponent<BombComponent>()->SetState(new DeathState(m_Scene), MathLib::EBombState::Death);
	}
}

void dae::ExplosionState::OnStart(GameObject* gameObject)
{
	m_TileId = gameObject->GetComponent<BombComponent>()->GetTileId();
	m_BombStrength = gameObject->GetComponent<BombComponent>()->GetBombStrength();

	// TODO trigger explosion & animation sequence
	auto comp{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<PathwayCreatorComponent>() };
	auto& pathways{ comp->GetPathways() };

	const auto path{ pathways.at(m_TileId) };
	glm::vec2 pos2{ path.Rect.x - path.Rect.w, path.Rect.y };
	for (int i = 1; i <= m_BombStrength; i++) {
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

	// TODO check overlaps for enemy or player death
}

void dae::ExplosionState::HandleExplosionPlacement(int& index, const std::map<int, dae::PathWay>& pathways, bool& outHitWall)
{
	if (pathways.find(index) == pathways.end()) return;
	if (index >= 0 && index < GridSize * GridSize) {
		if (pathways.at(index).PathState == MathLib::EPathState::Tile && !outHitWall) {
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

void dae::ExplosionState::HandleExplosionEnd(int& index)
{
	if (index >= 0 && index < GridSize * GridSize) {
		auto comp{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<PathwayCreatorComponent>() };
		comp->ActivatePathway(index);
	}
}

void dae::BombedState::OnStart(GameObject* pGameObject)
{
	if (auto enemyComp{ pGameObject->GetComponent<EnemyComponent>() }) {

		pGameObject->GetComponent<dae::AudioComponent>()->PlayPopSound();

		auto font{ ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 10) };
		auto pos{ pGameObject->GetTransform()->GetWorld().Position };

		auto rect{ pGameObject->GetComponent<TextureComponent>()->GetRect() };

		int score{ 0 };
		// TODO change score based on stats
		//enemyComp->GetEnemyType() == EEnemyType::Pooka ? score = 200 : score = 400;

		auto go{ std::make_shared<dae::GameObject>() };
		go->AddComponent(std::make_unique<dae::TextObjectComponent>(std::to_string(score), font));
		go->AddComponent(std::make_unique<dae::FloatingScoreComponent>(m_Scene, score, pos));
		m_Scene->Add(go);

		if (auto player{ pGameObject->GetComponent<EnemyComponent>()->GetPlayer() }) {
			player->GetComponent<ValuesComponent>()->IncreaseScore(score);
		}
	}

	if (auto playerComp{ pGameObject->GetComponent<PlayerComponent>() }) {
		playerComp->Reposition();
		pGameObject->GetComponent<TextureComponent>()->SetIsVisible(false);
		pGameObject->GetComponent<InputComponent>()->DisableMovement(true);
		pGameObject->GetComponent<EntityMovementComponent>()->DisableMovement(true);
		pGameObject->GetComponent<ValuesComponent>()->Damage();
	}
}

void dae::BombedState::Update(GameObject* pGameObject)
{
	m_DeathTimer -= Timer::GetInstance().GetDeltaTime();
	if (m_DeathTimer <= 0) {
		if (auto enemyComp{ pGameObject->GetComponent<EnemyComponent>() }) {
			enemyComp->SetState(new DeathState(m_Scene), MathLib::ELifeState::DEAD);
		}
		if (auto playerComp{ pGameObject->GetComponent<PlayerComponent>() }) {
			if (pGameObject->GetComponent<ValuesComponent>()->GetLives() > 0) {
				playerComp->SetState(new AliveState(m_Scene), MathLib::ELifeState::ALIVE);
			}
			else {
				playerComp->SetState(new DeathState(m_Scene), MathLib::ELifeState::DEAD);
			}
		}
	}
}

void dae::AliveState::OnStart(GameObject* pGameObject)
{
	if (!pGameObject) return;
	if (auto enemyComp{ pGameObject->GetComponent<EnemyComponent>() }) {
	}
	if (auto playerComp{ pGameObject->GetComponent<PlayerComponent>() }) {
		pGameObject->GetComponent<TextureComponent>()->SetIsVisible(true);
		pGameObject->GetComponent<InputComponent>()->DisableMovement(false);
		pGameObject->GetComponent<EntityMovementComponent>()->DisableMovement(false);
	}
}

void dae::AliveState::Update(GameObject* pGameObject)
{
	if (m_Scene && pGameObject && m_Ptr != reinterpret_cast<int*>(pGameObject)) {
		if (pGameObject->IsMarkedForDestroy()) return;
		auto enemies{ m_Scene->GetGameObjects(EnumStrings[Names::EnemyGeneral], false) };
		for (const auto& enemy : enemies) {
			if (enemy && !enemy->IsMarkedForDestroy()) {
				auto lifestate{ enemy->GetComponent<EnemyComponent>()->GetState() };
				if (lifestate != MathLib::BOMBED && lifestate != MathLib::DEAD) {
					if (pGameObject && m_Ptr != reinterpret_cast<int*>(pGameObject) && !pGameObject->IsMarkedForDestroy()) {
						if (MathLib::IsOverlapping(pGameObject->GetComponent<TextureComponent>()->GetRect(), enemy->GetComponent<TextureComponent>()->GetRect())) {
							pGameObject->GetComponent<PlayerComponent>()->SetState(new BombedState(m_Scene), MathLib::ELifeState::BOMBED);
						}
					}
				}
			}
		}
	}
}
