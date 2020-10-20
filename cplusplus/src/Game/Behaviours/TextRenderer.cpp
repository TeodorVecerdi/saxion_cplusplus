#include "TextRenderer.hpp"

#include "Core/Fonts.hpp"
#include "Core/GameObject.hpp"
#include "SpriteRenderer.hpp"

TextRenderer::TextRenderer(const std::string& fontName) : TextRenderer(default_identifier(this), fontName) {}

TextRenderer::TextRenderer(const std::string& identifier, const std::string& fontName) : ScriptableBehaviour(identifier) {
	setFont(fontName);
}

void TextRenderer::setText(const std::string& textString) { text.setString(textString); }

void TextRenderer::setFillColor(sf::Color color) { text.setFillColor(color); }
void TextRenderer::setOutlineColor(sf::Color color) { text.setOutlineColor(color); }

void TextRenderer::setFontSize(uint32_t fontSize) { text.setCharacterSize(fontSize); }

void TextRenderer::setFont(const std::string& fontName) { text.setFont(Fonts::getInstance()->GetFont(fontName)); }

void TextRenderer::setResizeReference(SpriteRenderer* resizeReference) {
    this->resizeReference = resizeReference;
}

std::string TextRenderer::getText() const { return text.getString(); }

sf::Color TextRenderer::getFillColor() const { return text.getFillColor(); }
sf::Color TextRenderer::getOutlineColor() const { return text.getOutlineColor(); }

uint32_t TextRenderer::getFontSize() const { return text.getCharacterSize();  }


void TextRenderer::onRender(sf::RenderWindow& window) const {
	window.draw(text);
}

void TextRenderer::onUpdate(sf::Time ts) {
    const auto renderTransform = owner->getRenderTransform();
    const auto localBounds = text.getLocalBounds();

    glm::vec2 spriteRect;
    if (resizeReference) spriteRect = resizeReference->getSize();
    else spriteRect = glm::vec2(localBounds.width, localBounds.height);
    text.setPosition(renderTransform.position.x + (spriteRect.x/2.0f -localBounds.width/2.0f), renderTransform.position.y + (spriteRect.y / 2.0f - localBounds.height / 2.0f));
    text.setScale(renderTransform.scale.x, renderTransform.scale.y);
    text.setRotation(renderTransform.rotation.z);

    

    text.setOrigin(0.5f * spriteRect.x * renderTransform.scale.x, .5f* spriteRect.y * renderTransform.scale.y);
}
