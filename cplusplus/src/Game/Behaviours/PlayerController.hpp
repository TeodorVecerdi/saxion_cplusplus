#pragma once
#include "Core/Scene/ScriptableBehaviour.hpp"

class Button;
class SpriteRenderer;
class Character;

class PlayerController final : public ScriptableBehaviour {
public:
	PlayerController(Character* character, Button& attackButton, Button& healButton, Button& focusButton);
	PlayerController(const std::string& identifier, Character* character, Button& attackButton, Button& healButton,
	                 Button& focusButton);
	void enableControls();
	void disableControls();
	Character* getPlayer();
protected:
	void onUpdate(sf::Time ts) override;
	void onStart() override;

private:
	Character* character;
	Button& attackButton;
	Button& healButton;
	Button& focusButton;
};
