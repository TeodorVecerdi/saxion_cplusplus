#pragma once
#include "Core/ScriptableBehaviour.hpp"

class SpriteRenderer;
class Character;

class PlayerController final : public ScriptableBehaviour {
public:
    PlayerController(Character* character, SpriteRenderer& spriteRenderer);
    PlayerController(const std::string& identifier, Character* character, SpriteRenderer& spriteRenderer);

    void setIdleState();
    void setHurtState();
    void setAttackState();
protected:
    void onUpdate(sf::Time ts) override;
    void onStart() override;

private:
    Character* character;
    SpriteRenderer& spriteRenderer;
    std::unique_ptr<sf::Texture> idleTexture;
    std::unique_ptr<sf::Texture> hurtTexture;
    std::unique_ptr<sf::Texture> attackTexture;
};
