#include "PathwayCreatorComponent.h"
#include "GameObject.h"
#include "EntityMovementComponent.h"
#include "Renderer.h"
#include "RockComponent.h"
#include <memory>

dae::PathwayCreatorComponent::~PathwayCreatorComponent()
{
	m_Pathways.clear();
}

void dae::PathwayCreatorComponent::AddPathway(int id, glm::vec2 pos, std::string type)
{
	auto go = std::make_shared<GameObject>();
	m_pScene->Add(go);
	GetGameObject()->AddChild(go.get());
	go->SetName(std::to_string(id));

	glm::vec2 pos2{ pos.x, pos.y };
	go->GetTransform()->Translate(pos2);

	auto comp{ std::make_unique<TextureComponent>() };
	comp->SetName(std::to_string(id));
	comp->SetTexture("Levels/blocker.png");
	auto component{ go->AddComponent(std::move(comp)) };
	//component->SetPosition(pos.x, pos.y);
	component->SetIsVisible(false);

	SDL_Rect rect{ static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(Cellsize), static_cast<int>(Cellsize )};
	PathWay pathWay{
		id,
		component,
		EPathState::Blocker,
		{ rect.x + rect.w / 2, rect.y + rect.h / 2 },
		rect,
	};


	if(type == "tile"){
		pathWay.PathState = EPathState::Tile;
	}
	else if (type == "spawn") {
		pathWay.PathState = EPathState::Spawn;
		m_Spawns.push_back(pathWay);
	}
	else if(type == "enemySpawn"){
		pathWay.PathState = EPathState::EnemySpawn;
		m_EnemySpawns.push_back(pathWay);
	}
	else if (type == "blocker") {
		component->SetIsVisible(true);
	}	
	else if (type == "breakable") {
		component->SetTexture("Levels/breakable.png");
		component->SetIsVisible(true);
	}

	m_Pathways.insert({ id, pathWay });
}
void dae::PathwayCreatorComponent::ActivatePathway(int id)
{
	m_Pathways[id].TextureComponent->SetIsVisible(true);
	m_Pathways[id].PathState = EPathState::Tile;
}

void dae::PathwayCreatorComponent::Update()
{
	HandleEntityTileOverlap();
}

void dae::PathwayCreatorComponent::HandleEntityTileOverlap()
{
	auto& children{ GetGameObject()->GetChildren() };
	//if (m_pCharacters.size() > 0) {

	//	for (const auto& character : m_pCharacters) {
	//		if (character->IsMarkedForDestroy()) continue;
	//		const auto& moveComp{ character->GetComponent<EntityMovementComponent>() };
	//		for (const auto& gameObj : children)
	//		{
	//			auto texComp{ gameObj->GetComponent<TextureComponent>() };
	//			if (MathLib::IsOverlapping(texComp->GetRect(), moveComp->GetCollider())) {
	//				moveComp->SetNextTileId(std::stoi(gameObj->GetName()));
	//			}
	//			if (MathLib::IsOverlapping(texComp->GetRect(), moveComp->GetCharacterCollider())) {
	//				moveComp->SetCurrentTileId(std::stoi(gameObj->GetName()));
	//			}
	//		}

	//		//moveComp->SetShouldDig(m_Pathways[moveComp->GetNextTileId()].PathState == EPathState::Blocker);
	//		//ActivatePathway(moveComp->GetCurrentTileId());
	//	}
	//}

	if (m_pEnemies.size() > 0) {
		for (const auto& character : m_pEnemies) {
			if (character->IsMarkedForDestroy()) continue;
			const auto& moveComp{ character->GetComponent<EntityMovementComponent>() };
			for (const auto& gameObj : children)
			{
				auto texComp{ gameObj->GetComponent<TextureComponent>() };
				if (MathLib::IsOverlapping(texComp->GetRect(), moveComp->GetCollider())) {
					moveComp->SetNextTileId(std::stoi(gameObj->GetName()));
				}
				if (MathLib::IsOverlapping(texComp->GetRect(), moveComp->GetCharacterCollider())) {
					moveComp->SetCurrentTileId(std::stoi(gameObj->GetName()));
				}
			}
		}
	}
	else {
		m_pCharacters = m_pScene->GetGameObjects(EnumStrings[PlayerGeneral], false);
		m_pEnemies = m_pScene->GetGameObjects(EnumStrings[EnemyGeneral], false);
	}
}

void dae::PathwayCreatorComponent::Init()
{
	m_pCharacters = m_pScene->GetGameObjects(EnumStrings[PlayerGeneral], false);
	m_pEnemies = m_pScene->GetGameObjects(EnumStrings[EnemyGeneral], false);
	HandleEntityTileOverlap();
}

void dae::PathwayCreatorComponent::Render() const
{
	for (auto path : m_Pathways) {
		auto rrect = path.second.Rect;
		if (path.second.PathState == EPathState::Blocker) {
			SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255);
		}
		else {
			SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 200, 0, 200);
		}
		SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rrect);
		SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 100, 100, 0, 200);
		SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &rrect);
	}
}
