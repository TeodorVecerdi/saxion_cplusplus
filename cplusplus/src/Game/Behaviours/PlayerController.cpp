#include "PlayerController.hpp"


#include "Core/Behaviours/SpriteRenderer.hpp"
#include "Game/Character.hpp"

PlayerController::PlayerController(Character* character, SpriteRenderer& spriteRenderer) : PlayerController(default_identifier(this), character, spriteRenderer){}

PlayerController::PlayerController(const std::string& identifier, Character* character, SpriteRenderer& spriteRenderer) : ScriptableBehaviour(identifier), character(character), spriteRenderer(spriteRenderer) {}

void PlayerController::setIdleState() {
    spriteRenderer.setTexture(idleTexture.get());
}

void PlayerController::setHurtState() {
    spriteRenderer.setTexture(hurtTexture.get());
}

void PlayerController::setAttackState() {
    spriteRenderer.setTexture(attackTexture.get());
}

void PlayerController::onUpdate(sf::Time ts) {}

void PlayerController::onStart() {
    idleTexture = std::make_unique<sf::Texture>();
    idleTexture->loadFromFile(string_format("assets/textures/characters/%s/head_idle.png", character->getCharacterType().c_str()));
    hurtTexture = std::make_unique<sf::Texture>();
    hurtTexture->loadFromFile(string_format("assets/textures/characters/%s/head_hurt.png", character->getCharacterType().c_str()));
    attackTexture = std::make_unique<sf::Texture>();
    attackTexture->loadFromFile(string_format("assets/textures/characters/%s/head_attack.png", character->getCharacterType().c_str()));

    setIdleState();
}
