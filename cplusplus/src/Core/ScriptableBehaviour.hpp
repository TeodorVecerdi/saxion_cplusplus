#pragma once
#include "Core.hpp"

class GameObject;

class ScriptableBehaviour {
public:
	ScriptableBehaviour();
	ScriptableBehaviour(std::string identifier);
	virtual ~ScriptableBehaviour() = default;
	virtual void update(sf::Time ts) final;
	virtual void render(sf::RenderWindow& window) const final;
	std::string getIdentifier() const;
	void setOwner(GameObject* newOwner);
	void resetRanStart();
protected:
	virtual void onStart() { }
	virtual void onRender(sf::RenderWindow& window) const {}
	virtual void onUpdate(const sf::Time ts) { }
	const std::string identifier;
	GameObject* owner;
private:
	bool ranStart = false;
};



