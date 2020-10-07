#pragma once
#include <SFML/Graphics.hpp>
class GameObject;

class ScriptableBehaviour {
public:
	ScriptableBehaviour(std::string identifier);
	virtual ~ScriptableBehaviour() = default;
	virtual void update(sf::Time ts) final;
	std::string getIdentifier() const;
	void setOwner(GameObject* newOwner);
protected:
	virtual void onStart() { }
	virtual void onUpdate(const sf::Time ts) { }
	const std::string identifier;
	GameObject* owner;
private:
	bool ranStart = false;
};



