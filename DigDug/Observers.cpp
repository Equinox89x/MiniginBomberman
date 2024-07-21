#include "Observers.h"
#include "ValuesComponent.h"
#include <string>
#include "Minigin.h"
#include "Observer.h"
#include "EntityMovementComponent.h"
#include "Commands.h"
#include "TextObjectComponent.h"
#include "SteamHolder.h"
#include <FileReader.h>
#include "PlayerComponent.h"

void dae::HealthObserver::Notify(GameObject* go, Event& event)
{
	auto i{ go->GetComponent<ValuesComponent>()->GetLives() };
	switch (event.GetEvent())
	{
	case EventType::Live:
		if (auto child{ m_Scene->GetGameObject(EnumStrings[Names::Life]) }) {
			child->GetComponent<TextObjectComponent>()->SetText(child->GetName() + " Lives: " + std::to_string(i));
		}
		if (i < 0) {
			m_Scene->GetGameObject(EnumStrings[Names::Global])->GetComponent<MenuComponent>()->GameOver();
		}

		break;
	case EventType::Reset:
		break;
	}
}

dae::ScoreObserver::~ScoreObserver()
{
	//if (g_SteamAchievements) {
	//	delete g_SteamAchievements;
	//	g_SteamAchievements = nullptr;
	//}
}

void dae::ScoreObserver::Notify(GameObject* player, Event& event)
{
	ValuesComponent* comp{ player->GetComponent<ValuesComponent>() };
	auto score{ comp->GetScores() };
	switch (event.GetEvent())
	{
	case EventType::Score:
		if (auto scoreHolder{m_Scene->GetGameObject(EnumStrings[Names::ScoreGeneral]+std::to_string(m_Id))}) {
			scoreHolder->GetComponent<TextObjectComponent>()->SetText(player->GetName() + " Score: " + std::to_string(score));
			scoreHolder->GetComponent<TextObjectComponent>()->SetName(std::to_string(score));
			//if (score == 500) {
			//	g_SteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
			//}
		}
		break;
	case EventType::Reset:
		if (auto scoreHolder{ m_Scene->GetGameObject(EnumStrings[Names::ScoreGeneral] + std::to_string(m_Id)) }) {
			scoreHolder->GetComponent<TextObjectComponent>()->SetText(player->GetName() + " Score: " + std::to_string(score));
			scoreHolder->GetComponent<TextObjectComponent>()->SetName(std::to_string(score));

		}
		break;

	}
}

//void dae::PumpEnemyOverlapObserver::Notify(GameObject* go, Event& event)
//{
//	auto enemies{ m_Scene->GetGameObject(EnumStrings[Names::EnemyHolder])->GetChildren() };
//
//}

void dae::DoorObserver::Notify(GameObject* /*go*/, Event& event)
{
	switch (event.GetEvent())
	{
	case EventType::DoorTrigger:
		auto enemies{ m_Scene->GetGameObjects(EnumStrings[Names::EnemyGeneral], false) };
		if (enemies.size() <= 1) {
			if (auto player{ m_Scene->GetGameObject(EnumStrings[Names::Player0]) }) {
				if (auto comp{ player->GetComponent<ValuesComponent>() }) {
					FileReader* file{ new FileReader("../Data/save.json") };
					file->WriteData({ {"Score", std::to_string(comp->GetScores())}, {"Lives", std::to_string(comp->GetLives()) } });
					delete file;
					auto go{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator]) };
					go->GetComponent<PathwayCreatorComponent>()->MarkForDestroy();
					m_Scene->Remove(go);
					//go->MarkForDestroy();

					m_Scene->GetGameObject(EnumStrings[Names::Global])->GetComponent<MenuComponent>()->SkipLevel();
				}
			}
		}
		break;
	}
}