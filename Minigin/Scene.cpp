#include "Scene.h"
#include "Scene.h"
#include "GameObject.h"
#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() {
	m_objects.clear();
}

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	if (object) {
		m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
	}
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

std::shared_ptr<GameObject> Scene::GetGameObject(std::string name) {
	for (const std::shared_ptr<GameObject>& obj : m_objects)
	{
		if (obj && obj->GetName() == name)
			return obj;
	}
	return nullptr;
}

std::vector<std::shared_ptr<GameObject>> Scene::GetGameObjects(std::string name, bool isCompleteWord)
{
	std::vector<std::shared_ptr<GameObject>> children;

	for (const std::shared_ptr<GameObject>& obj : m_objects)
	{
		if (obj) {
			if (!isCompleteWord) {
				std::size_t found = obj->GetName().find(name);
				if (found != std::string::npos) {
					children.push_back(obj);

				}
			}
			else if (obj->GetName() == name) {
				children.push_back(obj);
			}
		}

	}
	return children;
}

std::vector<std::shared_ptr<GameObject>>& dae::Scene::GetGameObjects()
{
	return m_objects;
}

void Scene::Update()
{
	for (auto& object : m_objects)
    {
		if (m_objects.size() <= 0) break;
		object->Update();
    }

	if (m_objects.size() > 0) {
		m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(),
			[](const std::shared_ptr<GameObject>& obj) { return obj && obj->IsMarkedForDestroy(); }),
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
	for (auto& obj : m_objects) {
		obj->MarkForDestroy();
	}

	m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(),
		[](const std::shared_ptr<GameObject>& obj) { return obj && obj->IsMarkedForDestroy(); }),
		m_objects.end());
}

void Scene::LateUpdate()
{
	for(auto& object : m_objects)
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

