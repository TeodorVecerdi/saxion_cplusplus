#include "TestBehaviour.hpp"

#include "Core/Behaviours/SpriteRenderer.hpp"

TestBehaviour::TestBehaviour(SpriteRenderer& sprite) :ScriptableBehaviour(default_identifier(this)), sprite(sprite) {}

void TestBehaviour::onStart() {
    fillAmount = 0.0f;
    fillDirection = 1;
    originalSize = sprite.getSize();
    sprite.setSmooth(true);
}

void TestBehaviour::onUpdate(sf::Time ts) {
    fillAmount +=  0.5f * fillDirection * ts.asSeconds();

    if(fillAmount < 0) {
        fillAmount = 0;
        fillDirection = -fillDirection;
    }
    if (fillAmount > 1) {
        fillAmount = 1;
        fillDirection = -fillDirection;
    }

    auto size = originalSize * glm::vec2(fillAmount, 1.0f);
    sprite.setSize(size);
}
