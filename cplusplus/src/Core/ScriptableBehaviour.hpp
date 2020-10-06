#pragma once
#include <SFML/Graphics.hpp>

class ScriptableBehaviour {
public:
	explicit ScriptableBehaviour(std::string identifier);
	virtual ~ScriptableBehaviour() = default;
	virtual void update(sf::Time ts) final;
	std::string getIdentifier() const;
protected:
	virtual void onStart() { }
	virtual void onUpdate(const sf::Time ts) { }
private:
	bool ranStart = false;
	const std::string identifier;
};