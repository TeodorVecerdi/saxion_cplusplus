#include "FilledSpriteRenderer.hpp"

FilledSpriteRenderer::FilledSpriteRenderer(const std::string& texturePath, bool fillDirection): SpriteRenderer(texturePath, fillDirection) {}

FilledSpriteRenderer::FilledSpriteRenderer(const std::string& identifier, const std::string& texturePath, bool fillDirection): SpriteRenderer(identifier, texturePath), fillVertical(fillDirection), fillAmount(1) {}

void FilledSpriteRenderer::setFillDirection(bool vertical) {
    fillVertical = vertical;
    updateFilledSize();
}

void FilledSpriteRenderer::setSize(const glm::vec2 size) {
    SpriteRenderer::setSize(size);
    originalSize = size;
    updateFilledSize();
}

void FilledSpriteRenderer::setFillAmount(float fillAmount) {
    this->fillAmount = fillAmount;
    updateFilledSize();
}

void FilledSpriteRenderer::updateFilledSize() {
    filledSize = originalSize * (fillVertical ? glm::vec2(1.0f, fillAmount) : glm::vec2(fillAmount, 1.0f));
}

void FilledSpriteRenderer::onUpdate(const sf::Time ts) {
    SpriteRenderer::setSize(filledSize);
    SpriteRenderer::onUpdate(ts);
}
