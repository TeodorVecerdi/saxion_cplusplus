﻿#include "PlayerController.hpp"

#include "Core/Behaviours/Button.hpp"
#include "Game/State/Character.hpp"

PlayerController::PlayerController(std::shared_ptr<Character> character, Button& attackButton, Button& healButton, Button& focusButton) : PlayerController(default_identifier(this), character, attackButton, healButton, focusButton){}

PlayerController::PlayerController(const std::string& identifier, std::shared_ptr<Character> character, Button& attackButton, Button& healButton, Button& focusButton) :
    ScriptableBehaviour(identifier), character(character), attackButton(attackButton), healButton(healButton), focusButton(focusButton) {
}

void PlayerController::enableControls() {
    attackButton.setEnabled(true);
    healButton.setEnabled(true);
    focusButton.setEnabled(true);
}

void PlayerController::disableControls() {
    attackButton.setEnabled(false);
    healButton.setEnabled(false);
    focusButton.setEnabled(false);
}

std::shared_ptr<Character> PlayerController::getPlayer() {
    return character;
}

