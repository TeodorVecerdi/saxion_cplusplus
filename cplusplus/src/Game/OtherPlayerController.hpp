#pragma once
#include "Core/ScriptableBehaviour.hpp"

class OtherPlayerController : public ScriptableBehaviour {
public:
	OtherPlayerController(const std::string& identifier, float speed, float rotationSpeed);
protected:
	void onUpdate(sf::Time ts) override;

private:
	float speed;
	float rotationSpeed;
};
