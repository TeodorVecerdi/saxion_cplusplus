#include "CharacterVisualizer.hpp"


#include "Core/Behaviours/FilledSpriteRenderer.hpp"
#include "Core/Behaviours/SpriteRenderer.hpp"
#include "Core/Behaviours/TextRenderer.hpp"
#include "Game/Character.hpp"

CharacterVisualizer::CharacterVisualizer(Character* character, SpriteRenderer* mainSprite,
                                         FilledSpriteRenderer* healthBar, TextRenderer* healthText, TextRenderer* nameText) :
	CharacterVisualizer(character, mainSprite, healthBar, healthText, nameText, nullptr, nullptr, nullptr) {
	visualizeLevel = false;
}

CharacterVisualizer::CharacterVisualizer(Character* character, SpriteRenderer* mainSprite,
                                         FilledSpriteRenderer* healthBar, TextRenderer* healthText, TextRenderer* nameText,
                                         FilledSpriteRenderer* levelBar, TextRenderer* levelText,
                                         TextRenderer* levelProgressText): state(IDLE), mainSprite(mainSprite),
                                                                           healthBar(healthBar), healthText(healthText),
                                                                           levelBar(levelBar), levelText(levelText),
                                                                           levelProgressText(levelProgressText), nameText(nameText) {
	this->character = character;
	idleTexture = std::make_unique<sf::Texture>();
	hurtTexture = std::make_unique<sf::Texture>();
	attackTexture = std::make_unique<sf::Texture>();
}

void CharacterVisualizer::setCharacter(Character* character) {
	this->character = character;
	if (character != nullptr) {
		idleTexture->loadFromFile(string_format("assets/textures/characters/%s/head_idle.png",
		                                        character->getCharacterType().c_str()));
		hurtTexture->loadFromFile(string_format("assets/textures/characters/%s/head_hurt.png",
		                                        character->getCharacterType().c_str()));
		attackTexture->loadFromFile(string_format("assets/textures/characters/%s/head_attack.png",
		                                          character->getCharacterType().c_str()));
		setState(state);
	}
}

void CharacterVisualizer::setState(CharacterState state) {
	if(state != IDLE && this->state != state)
		this->characterStateTimer = maintainStateTime;
	this->state = state;
	
	switch (state) {
	case HURT:
		mainSprite->setTexture(hurtTexture.get());
		break;
	case ATTACK:
		mainSprite->setTexture(attackTexture.get());
		break;
	default:
		mainSprite->setTexture(idleTexture.get());
		break;
	}
}

void CharacterVisualizer::onUpdate(const sf::Time ts) {
	if (character == nullptr) return;

	// Handle main sprite states
	if(state != IDLE) {
		characterStateTimer -= ts.asSeconds();
		if(characterStateTimer <= 0.0f) {
			setState(IDLE);
		}
	}

	// Health
	const int maxHealth = character->getMaxHealth();
	const int currentHealth = character->getHealth();
	const float currentHealthProgress = (float)currentHealth / maxHealth;
	healthBar->setFillAmount(currentHealthProgress);
	healthText->setText(string_format("%i/%i", currentHealth, maxHealth));

	// Name
	nameText->setText(character->getName());
	
	// Level
	// TODO: Add level
	if(!visualizeLevel) return;
	const int maxLevelProgress = character->getMaxHealth();
	const int currentLevelProgress = character->getHealth();
	const int currentLevel = character->getHealth();
	const float currentLevelProgressFill = (float)currentLevelProgress / maxLevelProgress;
	levelBar->setFillAmount(currentLevelProgressFill);
	levelProgressText->setText(string_format("%i/%i", currentLevelProgress, maxLevelProgress));
	levelText->setText(string_format("Level: %i", currentLevel));
}
