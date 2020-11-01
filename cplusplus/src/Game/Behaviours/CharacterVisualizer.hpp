#pragma once

#include "CharacterState.hpp"
#include "Core/Scene/ScriptableBehaviour.hpp"

class TextRenderer;
class FilledSpriteRenderer;
class SpriteRenderer;
class Character;

class CharacterVisualizer final : public ScriptableBehaviour {
public:
	CharacterVisualizer(std::shared_ptr<Character> character, SpriteRenderer* mainSprite,
	                    FilledSpriteRenderer* healthBar,
	                    TextRenderer* healthText, TextRenderer* nameText, TextRenderer* vitAttrLabel,
	                    TextRenderer* atkAttrLabel, TextRenderer* defAttrLabel, TextRenderer* stmAttrLabel);
	CharacterVisualizer(const std::string& identifier, std::shared_ptr<Character> character, SpriteRenderer* mainSprite,
	                    FilledSpriteRenderer* healthBar,
	                    TextRenderer* healthText, TextRenderer* nameText, TextRenderer* vitAttrLabel,
	                    TextRenderer* atkAttrLabel, TextRenderer* defAttrLabel, TextRenderer* stmAttrLabel);
	void setCharacter(std::shared_ptr<Character> character);
	void setState(CharacterState state);
protected:
	void onUpdate(sf::Time ts) override;
private:
	CharacterState state;
	float characterStateTimer;
	const float maintainStateTime = 2.0f;

	std::shared_ptr<Character> character;
	SpriteRenderer* mainSprite;
	FilledSpriteRenderer* healthBar;
	TextRenderer* healthText;
	TextRenderer* nameText;
	TextRenderer* vitAttrLabel;
	TextRenderer* atkAttrLabel;
	TextRenderer* defAttrLabel;
	TextRenderer* stmAttrLabel;

	std::unique_ptr<sf::Texture> idleTexture;
	std::unique_ptr<sf::Texture> hurtTexture;
	std::unique_ptr<sf::Texture> attackTexture;
};
