#include "CharacterVisualizer.hpp"

#include "Core/Behaviours/FilledSpriteRenderer.hpp"
#include "Core/Behaviours/SpriteRenderer.hpp"
#include "Core/Behaviours/TextRenderer.hpp"
#include "Game/State/Character.hpp"

CharacterVisualizer::CharacterVisualizer(std::shared_ptr<Character> character, SpriteRenderer* mainSprite,
                                         FilledSpriteRenderer* healthBar, TextRenderer* healthText,
                                         TextRenderer* nameText, TextRenderer* vitAttrLabel, TextRenderer* atkAttrLabel,
                                         TextRenderer* defAttrLabel, TextRenderer* stmAttrLabel) :
	CharacterVisualizer(default_identifier(this), character, mainSprite, healthBar, healthText, nameText, vitAttrLabel,
	                    atkAttrLabel, defAttrLabel, stmAttrLabel) {
}

CharacterVisualizer::CharacterVisualizer(const std::string& identifier, std::shared_ptr<Character> character, SpriteRenderer* mainSprite, FilledSpriteRenderer* healthBar,
										 TextRenderer* healthText, TextRenderer* nameText, TextRenderer* vitAttrLabel, TextRenderer* atkAttrLabel, TextRenderer* defAttrLabel,
										 TextRenderer* stmAttrLabel):
										 ScriptableBehaviour(identifier), state(IDLE), mainSprite(mainSprite), healthBar(healthBar), healthText(healthText), nameText(nameText),
										 vitAttrLabel(vitAttrLabel), atkAttrLabel(atkAttrLabel), defAttrLabel(defAttrLabel), stmAttrLabel(stmAttrLabel) {
	this->character = character;
	idleTexture = std::make_unique<sf::Texture>();
	hurtTexture = std::make_unique<sf::Texture>();
	attackTexture = std::make_unique<sf::Texture>();
}

void CharacterVisualizer::setCharacter(std::shared_ptr<Character> character) {
	this->character = character;
	if (character != nullptr) {
		idleTexture->loadFromFile(string_format("assets/textures/characters/%s/head_idle.png",
		                                        character->getCharacterType().c_str()));
		hurtTexture->loadFromFile(string_format("assets/textures/characters/%s/head_hurt.png",
		                                        character->getCharacterType().c_str()));
		attackTexture->loadFromFile(string_format("assets/textures/characters/%s/head_attack.png",
		                                          character->getCharacterType().c_str()));
		setState(state);
		vitAttrLabel->setText(string_format("%i", character->getVitality()));
		atkAttrLabel->setText(string_format("%i", character->getAttack()));
		defAttrLabel->setText(string_format("%i", character->getDefense()));
		stmAttrLabel->setText(string_format("%i", character->getStamina()));
	}
}

void CharacterVisualizer::setState(CharacterState state) {
	if (state != IDLE && this->state != state)
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
	if (state != IDLE) {
		characterStateTimer -= ts.asSeconds();
		if (characterStateTimer <= 0.0f) {
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
}
