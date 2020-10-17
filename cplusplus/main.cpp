#include "Core/FontCache.hpp"

#include "Core.hpp"

#include "Core/Input.hpp"
#include "Core/Scene.hpp"
#include "Core/SceneManager.hpp"
#include "Game/Button.hpp"
#include "Game/Label.hpp"

#include "Game/Sprite.hpp"
#include "Game/Behaviours/FPSCounter.hpp"
#include "Game/Behaviours/OtherPlayerController.hpp"
#include "Game/Behaviours/PlayerController.hpp"
#include "Game/Behaviours/SpriteRenderer.hpp"
#include "Game/Behaviours/TextRenderer.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Hello SFML");
    window.setVerticalSyncEnabled(true);
    window.setView(sf::View(sf::FloatRect(0.0f, 0.0f, 1280, 720)));
    sf::Clock deltaClock;

    // FPS
    sf::Font font;
    font.loadFromFile("res/arial.ttf");
    FontCache::RegisterFont("Arial", font);

    Label fpsText("", "Arial");
    fpsText.setFontSize(20);
    fpsText.setOrigin(glm::vec2(0, 0));
    GameObject blueBox;
    SpriteRenderer r1("res/square.png");
    r1.setColor(sf::Color::Blue);
    r1.setSize(glm::vec2(200));
    blueBox.addScriptableBehaviour(r1);
    blueBox.setOrigin(glm::vec2(0.5));
    blueBox.setPosition(glm::vec2(window.getSize().x / 2.0f, window.getSize().y/2.0f));
    GameObject redBox;
    PlayerController controller(100, 1);
    redBox.addScriptableBehaviour(controller);
    SpriteRenderer r2(r1);
    r2.setColor(sf::Color::Red);
    redBox.addScriptableBehaviour(r2);
    redBox.setOrigin(glm::vec2(0.5));
    redBox.setPosition(glm::vec2(window.getSize().x / 2.0f - 205, window.getSize().y / 2.0f));
    TextRenderer tr("Arial");
    tr.setAlignment(TextAlignment::MIDDLE_RIGHT);
    tr.setText("Hello, World!");
    redBox.addScriptableBehaviour(tr);

    /*Button button("Click me!", "Arial", "res/square.png", []() {
        printf("I was clicked\n");
    });*/
    // button.setOrigin(glm::vec2(0.5, 0.5));
    // button.setPosition(glm::vec2(window.getSize().x / 2.0, 100));
    // button.setScale(glm::vec2(2, 0.5));
    // button.setSize(glm::vec2(500, 100));

    Scene gameScene("gameScene");
    SceneManager sceneManager;
    sceneManager.registerScene(gameScene);
    sceneManager.stackScene(gameScene);

    // OtherPlayerController otherController("buttonController", 10.0f);
    FPSCounter fpsCounter("fpsCounter", 0.5f);

    // button.addScriptableBehaviour(otherController);
    fpsText.addScriptableBehaviour(fpsCounter);

    // gameScene.addChild(button);
    gameScene.addChild(blueBox);
    gameScene.addChild(redBox);
    gameScene.addChild(fpsText);

    while (window.isOpen()) {
        sf::Time timeStep = deltaClock.restart();
        sf::Event evt;
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

        sceneManager.update(timeStep);
        Input::resetHitCounters();

        window.clear();
        sceneManager.render(window);
        window.display();
    }
}
