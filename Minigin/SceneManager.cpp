#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Init()
{
	for (auto& scene : m_scenes)
	{
		scene->Init();
	}
}

void dae::SceneManager::Update()
{
	for(auto& scene : m_scenes)
	{
		if (scene->GetIsActive())
			scene->Update();
	}
}

void dae::SceneManager::LateUpdate()
{
	for (auto& scene : m_scenes)
	{
		if (scene->GetIsActive())
			scene->LateUpdate();
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		if (scene->GetIsActive())
			scene->Render();
	}
}

void dae::SceneManager::Cleanup()
{
	for (const auto& scene : m_scenes)
	{
		scene->Cleanup();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}

void dae::SceneManager::DeleteScene(const std::string& name)
{
	auto scene = std::find_if(m_scenes.begin(), m_scenes.end(), [&](const std::shared_ptr<dae::Scene>& scene) {
		return scene->GetName() == name;
		});

	if (scene != m_scenes.end()) {
		scene->get()->Cleanup();
		m_scenes.erase(std::remove(m_scenes.begin(), m_scenes.end(), *scene));
	}
}

void dae::SceneManager::DeleteScene(std::shared_ptr<dae::Scene> scene)
{
	if (scene) {
		scene.get()->Cleanup();
		m_scenes.erase(std::remove(m_scenes.begin(), m_scenes.end(), scene));
	}
}

std::shared_ptr<dae::Scene> dae::SceneManager::GetScene(int index)
{
	if (index - 1 <= static_cast<int>(m_scenes.size())) {
		return m_scenes[index];
	}
	else {
		return nullptr;
	}
}

std::shared_ptr<dae::Scene> dae::SceneManager::GetScene(std::string name)
{
	auto scene = std::find_if(m_scenes.begin(), m_scenes.end(), [&](const std::shared_ptr<dae::Scene>& scene) {
		return scene->GetName() == name;
	});

	if (scene != m_scenes.end()) {
		return *scene;
	}
	else {
		return nullptr;
	}
}

std::shared_ptr<dae::Scene> dae::SceneManager::GetActiveScene()
{
	auto scene = std::find_if(m_scenes.begin(), m_scenes.end(), [&](const std::shared_ptr<dae::Scene>& scene) {
		return scene->GetIsActive();
		});

	if (scene != m_scenes.end()) {
		return *scene;
	}
	else {
		return nullptr;
	}
}
