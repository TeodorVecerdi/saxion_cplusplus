#pragma once
#include "Core/ScriptableBehaviour.hpp"
#include "Sprite.hpp"

class PlayerController final : public ScriptableBehaviour {
public:
	PlayerController(const std::string& identifier, float speed, Sprite& sprite);
protected:
	void onUpdate(sf::Time ts) override;
	void onStart() override;

private:
	float speed;
	Sprite& sprite;
};
