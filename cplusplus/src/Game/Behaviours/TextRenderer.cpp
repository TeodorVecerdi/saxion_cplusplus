#include "TextRenderer.hpp"

#include "Core/FontCache.hpp"
#include "Core/GameObject.hpp"

TextRenderer::TextRenderer(const std::string& fontName) : TextRenderer(default_identifier(this), fontName) {}

TextRenderer::TextRenderer(const std::string& identifier, const std::string& fontName) : ScriptableBehaviour(identifier) {
	setFont(fontName);
}

void TextRenderer::setText(const std::string& textString) { text.setString(textString); }

void TextRenderer::setFillColor(sf::Color color) { text.setFillColor(color); }
void TextRenderer::setOutlineColor(sf::Color color) { text.setOutlineColor(color); }

void TextRenderer::setAlignment(TextAlignment alignment) { this->alignment = alignment; }

void TextRenderer::setFontSize(uint32_t fontSize) { text.setCharacterSize(fontSize); }

void TextRenderer::setFont(const std::string& fontName) { text.setFont(FontCache::GetFont(fontName)); }

std::string TextRenderer::getText() const { return text.getString(); }

sf::Color TextRenderer::getFillColor() const { return text.getFillColor(); }
sf::Color TextRenderer::getOutlineColor() const { return text.getOutlineColor(); }

TextAlignment TextRenderer::getAlignment() const { return alignment;  }

uint32_t TextRenderer::getFontSize() const { return text.getCharacterSize();  }


void TextRenderer::onRender(sf::RenderWindow& window) const {
	window.draw(text);
}

void TextRenderer::onUpdate(sf::Time ts) {
    const auto renderTransform = owner->getRenderTransform();
    text.setPosition(renderTransform.position.x, renderTransform.position.y);
    text.setScale(renderTransform.scale.x, renderTransform.scale.y);
    text.setRotation(renderTransform.rotation.z);
    const auto origin = AlignmentToOrigin(alignment);
    const auto spriteRect = text.getLocalBounds() ;
    text.setOrigin(origin.x * spriteRect.width * renderTransform.scale.x, origin.y * spriteRect.height * renderTransform.scale.y);
}
