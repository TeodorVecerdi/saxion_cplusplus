#pragma once
#include <SFML/Graphics.hpp>
#include "ScriptableBehaviour.hpp"

class GameObject {
public:
	explicit GameObject(std::string identifier);
	GameObject(const GameObject& other);
	virtual ~GameObject() = default;
	virtual void update(sf::Time ts) final;
	void render(sf::RenderWindow& window) const;
	std::string getIdentifier() const;
	void addScriptableBehaviour(ScriptableBehaviour& behaviour);
protected:
	virtual void onStart() = 0;
	virtual void onRender(sf::RenderWindow& window) const = 0;
	virtual void onUpdate(sf::Time ts) = 0;
private:
	const std::string identifier;
	bool ranStart;
	std::map<std::string, ScriptableBehaviour*> scriptableBehaviours;
};
