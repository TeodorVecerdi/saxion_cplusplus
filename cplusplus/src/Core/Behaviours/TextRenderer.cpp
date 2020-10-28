#include "TextRenderer.hpp"

#include "Core/Fonts.hpp"
#include "Core/Scene/GameObject.hpp"

TextRenderer::TextRenderer(const std::string& fontName) : TextRenderer(default_identifier(this), fontName) {}

TextRenderer::TextRenderer(const std::string& fontName, const std::string& text, const uint32_t fontSize, const TextAlignment textAlignment, const sf::Color fillColor) : TextRenderer(fontName) {
    setText(text);
    setFontSize(fontSize);
    setAlignment(textAlignment);
    setFillColor(fillColor);
}

TextRenderer::TextRenderer(const std::string& identifier, const std::string& fontName) : ScriptableBehaviour(identifier), alignment(TextAlignment::CENTER) {
    setFont(fontName);
}

void TextRenderer::setText(const std::string& textString) { text.setString(textString); }

void TextRenderer::setFillColor(const sf::Color color) { text.setFillColor(color); }

void TextRenderer::setOutlineColor(const sf::Color color) { text.setOutlineColor(color); }

void TextRenderer::setFontSize(const uint32_t fontSize) { text.setCharacterSize(fontSize); }

void TextRenderer::setFont(const std::string& fontName) { text.setFont(Fonts::getInstance()->GetFont(fontName)); }

void TextRenderer::setAlignment(const TextAlignment alignment) { this->alignment = alignment; }

std::string TextRenderer::getText() const { return text.getString(); }

sf::Color TextRenderer::getFillColor() const { return text.getFillColor(); }

sf::Color TextRenderer::getOutlineColor() const { return text.getOutlineColor(); }

uint32_t TextRenderer::getFontSize() const { return text.getCharacterSize(); }

sf::FloatRect TextRenderer::getGlobalBounds() const {
    return text.getGlobalBounds();
}

void TextRenderer::onRender(sf::RenderWindow& window) const {
    window.draw(text);
}

void TextRenderer::onUpdate(sf::Time ts) {
    const auto renderTransform = owner->getRenderTransform();
    const auto localBounds = text.getLocalBounds();

    text.setPosition(renderTransform.position.x, renderTransform.position.y);
    text.setScale(renderTransform.scale.x, renderTransform.scale.y);
    text.setRotation(renderTransform.rotation.z);
    glm::vec2 origin(localBounds.left + localBounds.width / 2.0f, localBounds.top + localBounds.height / 2.0f);
    if (alignment == TextAlignment::LEFT) origin.x -= localBounds.width / 2.0f;
    if (alignment == TextAlignment::RIGHT) origin.x += localBounds.width / 2.0f;
    text.setOrigin(origin.x * renderTransform.scale.x, origin.y * renderTransform.scale.y);
}
