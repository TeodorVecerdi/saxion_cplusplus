#pragma once
#include "Core.hpp"
#include "ScriptableBehaviour.hpp"
#include "Transform2D.hpp"
template <typename T> concept behaviour_type = std::is_base_of<ScriptableBehaviour, T>::value;
template <typename T, typename ... Args> concept constructible = std::is_constructible<T, Args...>::value;

class GameObject {
public:
	GameObject();
	GameObject(std::string identifier);
	GameObject(const GameObject& other);
	virtual ~GameObject();
	virtual void update(sf::Time ts) final;
	void render(sf::RenderWindow& window) const;
	std::string getIdentifier() const;

	template <behaviour_type T, typename ...Args> requires constructible<T, Args...>
	T& addScriptableBehaviour(Args&& ...args);
	template <behaviour_type T>
	T* getScriptableBehaviour();
	template <behaviour_type T>
	std::vector<T*> getScriptableBehaviours();
	template <behaviour_type T>
	bool hasScriptableBehaviour();
	void addScriptableBehaviour(ScriptableBehaviour& behaviour);
	ScriptableBehaviour* removeScriptableBehaviour(ScriptableBehaviour& behaviour);
	ScriptableBehaviour* removeScriptableBehaviour(const std::string& identifier);
	void addChild(GameObject& gameObject);
	bool removeChild(GameObject& gameObject);
	bool removeChild(const std::string& identifier);
	void destroy();
public: // TRANSFORM
	void setPosition(glm::vec2 newPosition);
	void movePosition(glm::vec2 deltaPosition);
	glm::vec2 getPosition() const;
	void setScale(glm::vec2 newScale);
	glm::vec2 getScale() const;
	void setOrigin(glm::vec2 newOrigin);
	glm::vec2 getOrigin() const;
	void setRotation(float newRotation);
	float getRotation() const;
	Transform2D getRenderTransform() const;
	inline static int indent = 0;
protected:
	virtual void onStart() {
	}

	virtual void onRender(sf::RenderWindow& window) const {
	}

	virtual void onUpdate(const sf::Time ts) {
	}

	Transform2D transform;
	const std::string identifier;
	GameObject* parent = nullptr;
	std::map<std::string, ScriptableBehaviour*> scriptableBehaviourMap;
	std::vector<ScriptableBehaviour*> scriptableBehaviours;
	std::map<std::string, GameObject*> childrenMap;
	std::vector<GameObject*> children;

private:
	bool ranStart;
	bool isDestroyed;
	void resetRanStart();
	friend class Scene;
};

template <behaviour_type T, typename ... Args> requires constructible<T, Args...>
T& GameObject::addScriptableBehaviour(Args&&... args) {
	T* t = new T(std::forward<Args>(args)...);
	addScriptableBehaviour(*t);
	return *t;
}

template <behaviour_type T>
T* GameObject::getScriptableBehaviour() {
	for (auto behaviour : scriptableBehaviours) {
		auto castBehaviour = dynamic_cast<T*>(behaviour);
		if (castBehaviour) return castBehaviour;
	}
	return nullptr;
}

template <behaviour_type T>
std::vector<T*> GameObject::getScriptableBehaviours() {
	std::vector<T*> values;
	for (auto behaviour : scriptableBehaviours) {
		auto castBehaviour = dynamic_cast<T*>(behaviour);
		if (castBehaviour) values.push_back(castBehaviour);
	}
	return values;
}

template <behaviour_type T>
bool GameObject::hasScriptableBehaviour() {
	return getScriptableBehaviour<T>() != nullptr;
}
