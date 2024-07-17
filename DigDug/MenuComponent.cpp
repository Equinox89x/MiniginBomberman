#include "MenuComponent.h"
#include <InputManager.h>
#include <TextObjectComponent.h>

void dae::MenuComponent::CycleGameMode(bool isMoveUp)
{
	if (m_ControlTimer > 0) return;
	if (!isMoveUp) {
		if (m_SelectedGameMode != MathLib::GameMode::VERSUS) {
			m_SelectedGameMode = MathLib::GameMode(static_cast<int>(m_SelectedGameMode) + 1);
		}
	}
	else {
		if (m_SelectedGameMode != MathLib::GameMode::SOLO) {
			m_SelectedGameMode = MathLib::GameMode(static_cast<int>(m_SelectedGameMode) - 1);
		}
	}
	m_ControlTimer = 0.2f;
	GetGameObject()->GetTransform()->TranslateWorld(m_CursorLocations[static_cast<int>(m_SelectedGameMode)]);

}

void dae::MenuComponent::StartGame(GameObject* /*pMenu*/)
{
	auto& manager = SceneManager::GetInstance();
	auto scene = manager.GetActiveScene();

	Input::GetInstance().ClearKeys();
	CreateLevel(m_SelectedGameMode, 0);

	manager.DeleteScene(scene);
}

void dae::MenuComponent::SkipLevel()
{
	auto& manager = SceneManager::GetInstance();
	auto scene = manager.GetActiveScene();

	Input::GetInstance().ClearKeys();
	if (m_SelectedGameMode == MathLib::GameMode::SOLO) {
		if (manager.GetScene(EnumStrings[SoloLevelName0])) {
			CreateLevel(m_SelectedGameMode, 1);
		}		
		else if (manager.GetScene(EnumStrings[SoloLevelName1])) {
			CreateLevel(m_SelectedGameMode, 2);
		}	
		else if (manager.GetScene(EnumStrings[SoloLevelName2])) {
			if (auto go{ m_Scene->GetGameObject(EnumStrings[Names::Score1]) }) {
				if (auto comp{ go->GetComponent<TextObjectComponent>() }) {
 					CreateHighscoreMenu();
				}
			}
		}
	}
	else {
		CreateHighscoreMenu();
	}

	manager.DeleteScene(scene);
}

void dae::MenuComponent::GameOver()
{
	auto& manager = SceneManager::GetInstance();
	auto scene = manager.GetActiveScene();
	Input::GetInstance().ClearKeys();
	CreateHighscoreMenu();
	manager.DeleteScene(scene);
}

void dae::MenuComponent::Update()
{
	m_ControlTimer -= Timer::GetInstance().GetDeltaTime();
}
