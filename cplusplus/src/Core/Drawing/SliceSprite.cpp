#include "SliceSprite.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

SliceSprite::SliceSprite() : m_texture(0), m_left(0), m_right(0), m_top(0), m_bottom(0), m_width(0), m_height(0),
                             m_need_update(true), m_vertex_array(sf::TrianglesStrip, 24) {
}

SliceSprite::~SliceSprite() {
}

sf::Vector2u SliceSprite::getSize() const {
	return sf::Vector2u(m_width, m_height);
}

void SliceSprite::setSize(sf::Vector2u size) {
	m_width = size.x;
	m_height = size.y;
	m_need_update = true;
}

void SliceSprite::setWidth(const uint32_t width) {
	m_width = width;
	m_need_update = true;
}

void SliceSprite::setHeight(const uint32_t height) {
	m_height = height;
	m_need_update = true;
}

void SliceSprite::setTexture(const sf::Texture& texture, const uint32_t border) {
	m_left = border;
	m_right = border;
	m_top = border;
	m_bottom = border;
	m_texture = &texture;
	m_need_update = true;
}

void SliceSprite::setBorder(const uint32_t border) {
	m_left = border;
	m_right = border;
	m_top = border;
	m_bottom = border;
	m_need_update = true;
}

void SliceSprite::setLeftBorder(const uint32_t border) {
	m_left = border;
	m_need_update = true;
}

void SliceSprite::setRightBorder(const uint32_t border) {
	m_right = border;
	m_need_update = true;
}

void SliceSprite::setTopBorder(const uint32_t border) {
	m_top = border;
	m_need_update = true;
}

void SliceSprite::setBottomBorder(const uint32_t border) {
	m_bottom = border;
	m_need_update = true;
}

uint32_t SliceSprite::leftBorder() const { return m_left; }
uint32_t SliceSprite::rightBorder() const { return m_right; }
uint32_t SliceSprite::topBorder() const { return m_top; }
uint32_t SliceSprite::bottomBorder() const { return m_bottom; }

void SliceSprite::setColor(const sf::Color& color) {
	for (int i = 0; i < 24; i++) {
		m_vertex_array[i].color = color;
	}
}

sf::Color SliceSprite::color() const {
	return m_vertex_array[0].color;
}

void SliceSprite::draw(sf::RenderTarget& target) const {
	draw(target, sf::RenderStates::Default);
}
void SliceSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (m_need_update == true) {
		updateVertices();
		m_need_update = false;
	}
	if (m_texture) {
		states.transform *= getTransform();
		states.texture = m_texture;
		target.draw(m_vertex_array, states);
	}
}

void SliceSprite::updateVertices() const {
	// http://dan.lecocq.us/wordpress/wp-content/uploads/2009/12/strip.png
	//     i ->
	//   j  0     1    2    3
	//   |
	//   V  4     5    6    7
	//
	//      8     9    10   11
	//
	//     12    13    14   15
	//
	// =>       0, 4, 1, 5, 2, 6, 3, 7, 7, 11, 6, 10, 5, 9 ,4 ,8, 8 ,12 ,9 ,13, 10 ,14, 11,15

	const uint32_t triangleStripVextexCount = 24;
	const uint32_t triangleStripVertexOrder[triangleStripVextexCount] = {
		0, 4, 1, 5, 2, 6, 3, 7, 7, 11, 6, 10, 5, 9, 4, 8, 8, 12, 9, 13, 10, 14, 11, 15
	};

	if (m_texture) {
		uint32_t x_pos[4] = {0, m_left, m_width - m_right, m_width};
		uint32_t y_pos[4] = {0, m_top, m_height - m_bottom, m_height};
		sf::Vector2f verticesPosition[16];

		uint32_t t_width = m_texture->getSize().x;
		uint32_t t_height = m_texture->getSize().x;

		uint32_t x_textCoord[4] = {0, m_left, t_width - m_right, t_width};
		uint32_t y_textCoord[4] = {0, m_top, t_height - m_bottom, t_height};
		sf::Vector2f verticesTextCoord[16];

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				verticesPosition[i + 4 * j] = sf::Vector2f(x_pos[i], y_pos[j]);
				verticesTextCoord[i + 4 * j] = sf::Vector2f(x_textCoord[i], y_textCoord[j]);
			}
		}

		for (int i = 0; i < triangleStripVextexCount; i++) {
			m_vertex_array[i].position = verticesPosition[triangleStripVertexOrder[i]];
			m_vertex_array[i].texCoords = verticesTextCoord[triangleStripVertexOrder[i]];
		}
	}
}
