#pragma once
#include "SpriteRenderer.hpp"

class FilledSpriteRenderer final : public SpriteRenderer {
public:
    FilledSpriteRenderer(const std::string& texturePath, bool fillDirection);
    FilledSpriteRenderer(const std::string& identifier, const std::string& texturePath, bool fillDirection);
    void setFillDirection(bool vertical);
    void setSize(glm::vec2 size) override;
    void setFillAmount(float fillAmount);
protected:
    void onUpdate(sf::Time ts) override;
private:
    void updateFilledSize();
    bool fillVertical; // false => horizontal, true => vertical;
    float fillAmount;
    glm::vec2 originalSize;
    glm::vec2 filledSize;
};
