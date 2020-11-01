#pragma once
#include "Core/Scene/ScriptableBehaviour.hpp"

class Button;
class SpriteRenderer;
class Character;

class PlayerController final : public ScriptableBehaviour {
public:
	PlayerController(std::shared_ptr<Character> character, Button& attackButton, Button& healButton, Button& focusButton);
	PlayerController(const std::string& identifier, std::shared_ptr<Character> character, Button& attackButton, Button& healButton,
	                 Button& focusButton);
	void enableControls();
	void disableControls();
	std::shared_ptr<Character> getPlayer();
private:
	std::shared_ptr<Character> character;
	Button& attackButton;
	Button& healButton;
	Button& focusButton;
};
