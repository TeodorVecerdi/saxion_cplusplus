#pragma once

#include "CharacterState.hpp"
#include "Core/Scene/ScriptableBehaviour.hpp"

class TextRenderer;
class FilledSpriteRenderer;
class SpriteRenderer;
class Character;

class CharacterVisualizer final : public ScriptableBehaviour {
public:
	CharacterVisualizer(Character* character, SpriteRenderer* mainSprite, FilledSpriteRenderer* healthBar,
	                    TextRenderer* healthText, TextRenderer* nameText);
	CharacterVisualizer(Character* character, SpriteRenderer* mainSprite, FilledSpriteRenderer* healthBar,
	                    TextRenderer* healthText, TextRenderer* nameText, FilledSpriteRenderer* levelBar, TextRenderer* levelText,
	                    TextRenderer* levelProgressText);
	void setCharacter(Character* character);
	void setState(CharacterState state);
protected:
	void onUpdate(const sf::Time ts) override;
private:
	bool visualizeLevel = true;
	CharacterState state;
	float characterStateTimer;
	const float maintainStateTime = 2.0f;
	
	Character* character;
	SpriteRenderer* mainSprite;
	FilledSpriteRenderer* healthBar;
	TextRenderer* healthText;
	TextRenderer* nameText;
	FilledSpriteRenderer* levelBar;
	TextRenderer* levelText;
	TextRenderer* levelProgressText;

	std::unique_ptr<sf::Texture> idleTexture;
	std::unique_ptr<sf::Texture> hurtTexture;
	std::unique_ptr<sf::Texture> attackTexture;
};
