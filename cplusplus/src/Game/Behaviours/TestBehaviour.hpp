#pragma once
#include "Core/Scene/ScriptableBehaviour.hpp"

class SpriteRenderer;

class TestBehaviour : public ScriptableBehaviour {
public:
    TestBehaviour(SpriteRenderer& sprite);
protected:
    void onStart() override;
    void onUpdate(sf::Time ts) override;
private:
    glm::vec2 originalSize;
    SpriteRenderer& sprite;
    float fillAmount;
    int fillDirection;
};
