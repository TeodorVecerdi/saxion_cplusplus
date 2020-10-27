#pragma once
#include "Core/Drawing/SliceSprite.hpp"
#include "Core/Scene/ScriptableBehaviour.hpp"

class SpriteRenderer : public ScriptableBehaviour {
public:
    SpriteRenderer(const std::string& texturePath, bool setSmooth = false);
    SpriteRenderer(const std::string& identifier, const std::string& texturePath, bool setSmooth = false);
    void setTexture(const std::string& texturePath, uint32_t spliceBorder = 8);
    void setTexture(const sf::Texture* texture, uint32_t spliceBorder = 8);
    virtual void setSize(glm::vec2 size);
    void setSmooth(bool smooth);
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
    bool smooth;
};
