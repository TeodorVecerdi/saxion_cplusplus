#include <SFML/Graphics.hpp>
#include "Core/Input.hpp"
#include "Core/Scoreboard.hpp"
#include "Game/PlayerController.hpp"
#include "Game/Sprite.hpp"
#include "Utils/StringUtils.hpp"

int main() {
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Hello SFML");
	window.setVerticalSyncEnabled(true);
	window.setView(sf::View(sf::FloatRect(0.0f, 0.0f, 1280, 720)));
	sf::Clock deltaClock;

	// FPS
	sf::Font font;
	font.loadFromFile("res/arial.ttf");
	sf::Text fpsText("", font);
	fpsText.setOrigin(0, 0);
	float minFPS = 100000000.0f;
	float maxFPS = 0.0f;
	float averageFPS = 0.0f;
	int count = 0;

	Sprite s("someSprite", "res/testTexture.png");
	PlayerController controller("someSprite_controller", 500.0f, s);
	s.addScriptableBehaviour(controller);

	while (window.isOpen()) {
		sf::Time timeStep = deltaClock.restart();
		sf::Event evt{};
		while (window.pollEvent(evt)) {
			if (evt.type == sf::Event::KeyPressed || evt.type == sf::Event::KeyReleased) {
				Input::setKey(evt.key.code, evt.type);
			}

			if (evt.type == sf::Event::MouseButtonPressed || evt.type == sf::Event::MouseButtonReleased) {
				Input::setButton(evt.mouseButton.button, evt.type);
			}

			if (evt.type == sf::Event::MouseMoved) {
				Input::updateMouse(evt.mouseMove.x, evt.mouseMove.y);
			}

			if (evt.type == sf::Event::Closed || (evt.type == sf::Event::KeyPressed && evt.key.code == sf::Keyboard::Key::Escape)) {
				window.close();
			}
		}
		
		float fps = 1.0f / timeStep.asSeconds();
		count ++;
		averageFPS = averageFPS + (fps - averageFPS) / count;
		if (count >= 60) count = 1;
		if (fps < minFPS) minFPS = fps;
		if (fps > maxFPS) maxFPS = fps;
		fpsText.setString(string_format("AVG: %-10.2f\tMIN: %-10.2f\tMAX: %-10.2f", averageFPS, minFPS, maxFPS));

		s.update(timeStep);

		Input::resetHitCounters();

		window.clear();
		s.render(window);
		window.draw(fpsText);
		window.display();
	}
}
