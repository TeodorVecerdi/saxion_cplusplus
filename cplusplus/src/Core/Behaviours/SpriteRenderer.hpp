#pragma once
#include "Core/ScriptableBehaviour.hpp"

class SpriteRenderer final : public ScriptableBehaviour {
public:
    SpriteRenderer(const std::string& texturePath, bool setSmooth = false);
    SpriteRenderer(const std::string& identifier, const std::string& texturePath, bool setSmooth = false);
    void setTexture(const std::string& texturePath);
    void setSize(glm::vec2 size);
    glm::vec2 getSize() const;
    void setColor(sf::Color color);
    sf::Color getColor() const;
    sf::FloatRect getGlobalBounds() const;
protected:
    void onRender(sf::RenderWindow& window) const override;
    void onUpdate(sf::Time ts) override;
private:
    void recalculateSizeMultiplier();
    glm::vec2 sizeMultiplier;
    glm::vec2 size;
    glm::vec2 textureSize;
    sf::Texture texture;
    sf::Sprite sprite;
};
