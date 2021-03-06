﻿#pragma once
#include "Core/Core.hpp"

class GameObject;

class ScriptableBehaviour {
public:
	ScriptableBehaviour();
	ScriptableBehaviour(std::string identifier);
	virtual ~ScriptableBehaviour();
	virtual void update(sf::Time ts) final;
	virtual void render(sf::RenderWindow& window) const final;
	std::string getIdentifier() const;
	void setOwner(GameObject* newOwner);
	void resetRanStart();
	void destroy();
protected:
	virtual void onStart() { }
	virtual void onRender(sf::RenderWindow& window) const {}
	virtual void onUpdate(const sf::Time ts) { }
	const std::string identifier;
	GameObject* owner;
private:
	bool ranStart;
	bool isDestroyed;
	friend class Scene;
	friend class GameObject;
};



