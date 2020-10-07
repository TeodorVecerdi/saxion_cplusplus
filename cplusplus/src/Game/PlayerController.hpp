#pragma once
#include "Core/ScriptableBehaviour.hpp"
#include "Sprite.hpp"

class PlayerController final : public ScriptableBehaviour {
public:
	PlayerController(const std::string& identifier, float speed, float rotationSpeed);
protected:
	void onUpdate(sf::Time ts) override;

private:
	float speed;
	float rotationSpeed;
};
