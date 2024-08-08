#include "Observers.h"
#include "Commands.h"
#include "EntityMovementComponent.h"
#include "Minigin.h"
#include "Observer.h"
#include "PlayerComponent.h"
#include "SteamHolder.h"
#include "TextObjectComponent.h"
#include "ValuesComponent.h"
#include <FileReader.h>
#include <string>

void dae::HealthObserver::Notify(GameObject* go, Event& event)
{
	auto i{ go->GetComponent<ValuesComponent>()->GetLives() };
	auto* global{ m_Scene->GetGameObject(EnumStrings[Names::Global]) };
	auto* livesHolder{ global->GetChild(EnumStrings[Names::Life] + std::to_string(m_Id)) };

	switch (event.GetEvent())
	{
	case EventType::Live:
		if (livesHolder)
		{
			livesHolder->GetComponent<TextObjectComponent>()->SetText(go->GetName() + " Lives: " + std::to_string(i));
		}
		//if (i <= 0)
		//{
		//	global->GetComponent<MenuComponent>()->GameOver();
		//}

		break;
	case EventType::Reset:
		break;
	}
}

dae::ScoreObserver::~ScoreObserver()
{
	// if (g_SteamAchievements) {
	//	delete g_SteamAchievements;
	//	g_SteamAchievements = nullptr;
	// }
}

void dae::ScoreObserver::Notify(GameObject* player, Event& event)
{
	ValuesComponent* comp{ player->GetComponent<ValuesComponent>() };
	auto			 score{ comp->GetScores() };
	auto*			 global{ m_Scene->GetGameObject(EnumStrings[Names::Global]) };
	auto			 scoreHolder{ global->GetChild(EnumStrings[Names::ScoreGeneral] + std::to_string(m_Id)) };
	switch (event.GetEvent())
	{
	case EventType::Score:
		if (scoreHolder)
		{
			scoreHolder->GetComponent<TextObjectComponent>()->SetText(player->GetName() + " Score: " + std::to_string(score));
			scoreHolder->GetComponent<TextObjectComponent>()->SetName(std::to_string(score));
			// if (score == 500) {
			//	g_SteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
			// }
		}
		break;
	case EventType::Reset:
		if (scoreHolder)
		{
			scoreHolder->GetComponent<TextObjectComponent>()->SetText(player->GetName() + " Score: " + std::to_string(score));
			scoreHolder->GetComponent<TextObjectComponent>()->SetName(std::to_string(score));
		}
		break;
	}
}

// void dae::PumpEnemyOverlapObserver::Notify(GameObject* go, Event& event)
//{
//	auto enemies{ m_Scene->GetGameObject(EnumStrings[Names::EnemyHolder])->GetChildren() };
//
// }

void dae::DoorObserver::Notify(GameObject* /*go*/, Event& event)
{
	switch (event.GetEvent())
	{
	case EventType::DoorTrigger:
		auto enemies{ m_Scene->GetGameObjects(EnumStrings[Names::EnemyGeneral], false) };
		if (enemies.size() <= 1)
		{
			if (auto player{ m_Scene->GetGameObject(EnumStrings[Names::Player0]) })
			{
				if (auto comp{ player->GetComponent<ValuesComponent>() })
				{
					FileReader* file{ new FileReader("../Data/save.json") };
					file->WriteData({ { "Score", std::to_string(comp->GetScores()) }, { "Lives", std::to_string(comp->GetLives()) } });
					delete file;
					auto go{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator]) };
					go->GetComponent<PathwayCreatorComponent>()->MarkForDestroy();
					m_Scene->Remove(go);
					// go->MarkForDestroy();

					m_Scene->GetGameObject(EnumStrings[Names::Global])->GetComponent<MenuComponent>()->SkipLevel();
				}
			}
		}
		break;
	}
}