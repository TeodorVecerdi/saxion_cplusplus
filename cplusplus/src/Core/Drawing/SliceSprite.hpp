#pragma once

namespace sf {
	class Texture;
	class Color;
}

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

// Taken from: https://en.sfml-dev.org/forums/index.php?topic=13590.0
class SliceSprite : public sf::Drawable, public sf::Transformable {
public:
	SliceSprite();
	virtual ~SliceSprite();

	sf::Vector2u getSize() const;
	void setSize(sf::Vector2u size);
	void setWidth(uint32_t width);
	void setHeight(uint32_t height);

	void setTexture(const sf::Texture& texture, uint32_t border);

	void setBorder(uint32_t border);
	void setLeftBorder(uint32_t leftBorder);
	void setRightBorder(uint32_t rightBorder);
	void setTopBorder(uint32_t upBorder);
	void setBottomBorder(uint32_t downBorder);
	uint32_t leftBorder() const;
	uint32_t rightBorder() const;
	uint32_t topBorder() const;
	uint32_t bottomBorder() const;

	void setColor(const sf::Color& color);
	sf::Color color() const;

private:
	const sf::Texture* m_texture;
	uint32_t m_left;
	uint32_t m_right;
	uint32_t m_top;
	uint32_t m_bottom;
	uint32_t m_width;
	uint32_t m_height;

	mutable sf::VertexArray m_vertex_array;
	mutable bool m_need_update;

	void updateVertices() const;
	virtual void draw(sf::RenderTarget& target) const;
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
