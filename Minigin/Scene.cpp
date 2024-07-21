#include "Scene.h"
#include "Scene.h"
//#include "GameObject.h"
#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() {
	m_objects.clear();
}

void Scene::Add(std::unique_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(GameObject* object)
{
	//if (object) {
	//	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
	//}

	if (object) {
		m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(),
				[&](const std::unique_ptr<GameObject>& ptr) {
					if (ptr.get() == object) {
						ptr->MarkForDestroy();
						return true;
					}
					return false;
				}),
			m_objects.end());
	}
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

GameObject* Scene::GetGameObject(std::string name) {
	for (const std::unique_ptr<GameObject>& obj : m_objects)
	{
		if (obj && obj->GetName() == name)
			return obj.get();
	}
	return nullptr;
}

std::vector<GameObject*> Scene::GetGameObjects(std::string name, bool isCompleteWord)
{
	std::vector<GameObject*> children;

	for (const std::unique_ptr<GameObject>& obj : m_objects)
	{
		if (obj) {
			if (!isCompleteWord) {
				std::size_t found = obj->GetName().find(name);
				if (found != std::string::npos) {
					children.push_back(obj.get());

				}
			}
			else if (obj->GetName() == name) {
				children.push_back(obj.get());
			}
		}

	}
	return children;
}

std::vector<GameObject*> dae::Scene::GetGameObjects()
{
	std::vector<GameObject*> children;

	for (const std::unique_ptr<GameObject>& obj : m_objects)
	{
		children.push_back(obj.get());
	}
	return children;
}

void Scene::Update()
{
	if (!m_IsActive) return;
	for (auto& object : m_objects)
	{
		if (m_objects.size() <= 0) break;
		object->Update();
	}

	if (m_objects.size() > 0) {
		m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(),
			[](const std::unique_ptr<GameObject>& obj) { return obj && obj->IsMarkedForDestroy(); }),
			m_objects.end());
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

void dae::Scene::Cleanup()
{
	m_IsActive = false;

	for (auto& obj : m_objects) {
		obj->MarkForDestroy();
	}

	m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(),
		[](const std::unique_ptr<GameObject>& obj) { return obj && obj->IsMarkedForDestroy(); }),
		m_objects.end());
}

void Scene::LateUpdate()
{
	for (auto& object : m_objects)
	{
		object->LateUpdate();
	}
}

void Scene::Init() const
{
	for (const auto& object : m_objects)
	{
		object->Init();
	}
}

