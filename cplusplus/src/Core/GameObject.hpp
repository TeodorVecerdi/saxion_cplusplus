#pragma once
#include <SFML/Graphics.hpp>
#include "ScriptableBehaviour.hpp"
#include <glm/glm.hpp>

#include "Transform2D.hpp"

class GameObject {
public:
	explicit GameObject(std::string identifier);
	GameObject(const GameObject& other);
	virtual ~GameObject() = default;
	virtual void update(sf::Time ts) final;
	void render(sf::RenderWindow& window) const;
	std::string getIdentifier() const;
public:
	void addScriptableBehaviour(ScriptableBehaviour& behaviour);
	bool removeScriptableBehaviour(ScriptableBehaviour& behaviour);
	bool removeScriptableBehaviour(const std::string& identifier);
	void addChild(GameObject& gameObject);
	bool removeChild(GameObject& gameObject);
	bool removeChild(const std::string& identifier);
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
protected:
	virtual void onStart() = 0;
	virtual void onRender(sf::RenderWindow& window) const = 0;
	virtual void onUpdate(sf::Time ts) = 0;
	Transform2D transform;
	const std::string identifier;
	GameObject* parent = nullptr;
private:
	bool ranStart;
	std::map<std::string, ScriptableBehaviour*> scriptableBehaviours;
	std::map<std::string, GameObject*> children;
};
