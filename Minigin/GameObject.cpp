#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TextureComponent.h"

dae::GameObject::GameObject()
{
	AddComponent(std::make_unique<TransformComponent>());
	m_pTransform = GetComponent<TransformComponent>();
}

dae::GameObject::~GameObject() {
	//for (size_t i = 0; i < m_pComponents.size(); i++)
	//{
	//	//m_pComponents[i].release();
	//	if (m_pComponents[i]) {
	//		m_pComponents[i].reset();
	//	}
	//	//m_pComponents.erase(std::remove(m_pComponents.begin(), m_pComponents.end(), m_pComponents[i]));
	//}	
	//
	//for (size_t i = 0; i < m_pChildren.size(); i++)
	//{
	//	m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), m_pChildren[i]));
	//}


	//m_pComponents.clear();
	//m_pChildren.clear();

	//m_pParent = nullptr;
	//m_pTransform = nullptr;

	//
};

void dae::GameObject::Init()
{
	for (const std::unique_ptr<Component>& comp : m_pComponents) {
		comp->Init();
	}

	for (GameObject* child : m_pChildren) {
		child->Init();
	}
}

void dae::GameObject::Update() {
	if (auto comp{ GetComponent<TextureComponent>() }) {
		if (GetTransform()) {
			m_Center.x = comp->GetRect().x + (comp->GetRect().w/1.75f);
			m_Center.y = GetTransform()->GetFullPosition().y + comp->GetRect().h / 2.3f;
		}
	}

	for (auto& comp : m_pComponents)
	{
		if (m_pComponents.size() <= 0) break;
		if(!comp->IsMarkedForDestroy()) comp->Update();
	}

	for (auto& child : m_pChildren)
	{
		child->Update();
	}

	if (m_pComponents.size() > 0) {
		m_pComponents.erase(std::remove_if(m_pComponents.begin(), m_pComponents.end(),
			[](const std::unique_ptr<Component>& comp) { return comp->IsMarkedForDestroy(); }),
			m_pComponents.end());
	}

	if (m_pChildren.size() > 0) {
		m_pChildren.erase(std::remove_if(m_pChildren.begin(), m_pChildren.end(),
			[](GameObject* child) { return child->IsMarkedForDestroy(); }),
			m_pChildren.end());
	}
}

void dae::GameObject::LateUpdate()
{
	for (const std::unique_ptr<Component>& comp : m_pComponents) {
		comp->LateUpdate();
	}

	for (GameObject* child : m_pChildren) {
		child->LateUpdate();
	}
}

void dae::GameObject::Render() const
{
	for (const std::unique_ptr<Component>& comp : m_pComponents) {
		comp->Render();
	}

	for (GameObject* child : m_pChildren) {
		child->Render();
	}
}

void dae::GameObject::Cleanup()
{
	//for (size_t i = 0; i < m_pComponents.size(); i++)
	//{
	//	//m_pComponents[i].release();
	//	if (m_pComponents[i]) {
	//		m_pComponents[i].reset();
	//	}
	//	//m_pComponents.erase(std::remove(m_pComponents.begin(), m_pComponents.end(), m_pComponents[i]));
	//}

	//for (size_t i = 0; i < m_pChildren.size(); i++)
	//{
	//	m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), m_pChildren[i]));
	//}


	//m_pComponents.clear();
	//m_pChildren.clear();

	for (auto& comp : m_pComponents)
	{
		comp->MarkForDestroy();
	}

	for (auto& child : m_pChildren)
	{
		child->MarkForDestroy();
	}

	m_pComponents.erase(std::remove_if(m_pComponents.begin(), m_pComponents.end(),
		[](const std::unique_ptr<Component>& comp) { return comp && comp->IsMarkedForDestroy(); }),
		m_pComponents.end());

	m_pChildren.erase(std::remove_if(m_pChildren.begin(), m_pChildren.end(),
		[](GameObject* child) { return child && child->IsMarkedForDestroy(); }),
		m_pChildren.end());

	m_pParent = nullptr;
	m_pTransform = nullptr;


}

void dae::GameObject::RemoveComponent(const std::unique_ptr<Component>& comp)
{
	if (comp) {
		m_pComponents.erase(std::remove(m_pComponents.begin(), m_pComponents.end(), comp));
	}
}

void dae::GameObject::SetParent(GameObject* const child, bool updateTransforms)
{
	//recursive child check, check slides too
	if (child) {
		//Remove itself as a child from the previous parent(if any).
		if (child->m_pParent) {
			child->m_pParent->m_pChildren.erase(std::remove(child->m_pParent->m_pChildren.begin(), child->m_pParent->m_pChildren.end(), child));
		}

		if (updateTransforms) {
			//Update position, rotation and scale
			m_pTransform->SetDirty();
		}

		//Set the given parent on itself.
		child->m_pParent = this;
		//Add itself as a child to the given parent.
		m_pChildren.push_back(child);
	}
}

dae::GameObject* dae::GameObject::GetParent() const
{
	return m_pParent;
}

void dae::GameObject::RemoveChild(GameObject* gameObject)
{
	if (gameObject) {
		auto it = find(m_pChildren.begin(), m_pChildren.end(), gameObject);
		if (it != m_pChildren.end()) {
			//	Remove itself as a parent of the child.
			gameObject->m_pParent = nullptr;

			//Remove the given child from the children list
			m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), gameObject));

			//	Update position, rotation and scale
			m_pTransform->UpdateTransforms();
		}
	}
}

dae::GameObject* dae::GameObject::AddChild(GameObject* gameObject, bool updateTransforms)
{
	if (gameObject) {
		// Remove the given child from the child's previous parent if it had a parent before
		if (gameObject->m_pParent) {
			gameObject->m_pParent->RemoveChild(gameObject);
		}

		//Set itself as parent of the child
		gameObject->m_pParent = this;


		//Update position, rotation and scale
		if (updateTransforms) {
			gameObject->GetTransform()->UpdateTransforms();
		}

		gameObject->Init();

		//Add the child to its children list.
		m_pChildren.push_back(std::move(gameObject));
	}

	return gameObject;
}

dae::GameObject* dae::GameObject::GetChild(std::string name) {
	for (auto child : m_pChildren)
	{
		if (child && child->GetName() == name)
			return child;
	}
	return nullptr;
}

std::vector<dae::GameObject*> dae::GameObject::GetChildren(std::string name)
{
	std::vector<GameObject*> children;

	for (auto child : m_pChildren)
	{
		if (child && child->GetName() == name)
			children.push_back(child);
	}
	return children;
}