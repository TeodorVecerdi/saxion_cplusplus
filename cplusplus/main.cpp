#include "Core.hpp"

#include "Core/Input.hpp"
#include "Core/Scene.hpp"
#include "Core/SceneManager.hpp"

#include "Game/Behaviours/FPSCounter.hpp"
#include "Game/Behaviours/PlayerController.hpp"
#include "Game/Behaviours/SpriteRenderer.hpp"
#include "Game/Behaviours/TextRenderer.hpp"
#include "Game/Behaviours/Button.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Hello SFML");
    window.setVerticalSyncEnabled(true);
    sf::Clock deltaClock;

    Scene gameScene("game");
    Scene mainMenu("mainMenu");
    SceneManager sceneManager;
    sceneManager.registerScene(gameScene);
    sceneManager.registerScene(mainMenu);
    sceneManager.stackScene(mainMenu);


    TextRenderer fpsText("Arial");
    fpsText.setFontSize(16);
    fpsText.setOutlineColor(sf::Color::Black);
    FPSCounter fpsCounter("fpsCounter", 0.5f);
    GameObject fpsTextObject;
    fpsTextObject.setPosition(glm::vec2(window.getSize().x / 2.0f, 20));
    fpsTextObject.addScriptableBehaviour(fpsText);
    fpsTextObject.addScriptableBehaviour(fpsCounter);

    
    SpriteRenderer exitButtonSprite("res/square.png");
    exitButtonSprite.setSize(glm::vec2(200, 50));
    TextRenderer exitButtonLabel("Arial");
    exitButtonLabel.setFontSize(30);
    exitButtonLabel.setFillColor(sf::Color::Blue);
    exitButtonLabel.setText("Exit game");
    exitButtonLabel.setResizeReference(&exitButtonSprite);
    Button exitButton(exitButtonSprite, [&window]() { window.close(); });
    GameObject exitButtonObject;
    exitButtonObject.setPosition(glm::vec2(window.getSize().x / 2.0f, 175));
    exitButtonObject.addScriptableBehaviour(exitButtonSprite);
    exitButtonObject.addScriptableBehaviour(exitButtonLabel);
    exitButtonObject.addScriptableBehaviour(exitButton);

    SpriteRenderer playButtonSprite("res/square.png");
    playButtonSprite.setSize(glm::vec2(200, 50));
    TextRenderer playButtonLabel("Arial");
    playButtonLabel.setFontSize(30);
    playButtonLabel.setFillColor(sf::Color::Blue);
    playButtonLabel.setText("Play game");
    playButtonLabel.setResizeReference(&playButtonSprite);
    Button playButton(playButtonSprite, [&sceneManager]() { sceneManager.stackScene("game"); });
    GameObject playButtonObject;
    playButtonObject.setPosition(glm::vec2(window.getSize().x/2.0f, 100));
    playButtonObject.addScriptableBehaviour(playButtonSprite);
    playButtonObject.addScriptableBehaviour(playButtonLabel);
    playButtonObject.addScriptableBehaviour(playButton);

    SpriteRenderer backButtonSprite("res/square.png");
    backButtonSprite.setSize(glm::vec2(200, 50));
    TextRenderer backButtonLabel("Arial");
    backButtonLabel.setFontSize(30);
    backButtonLabel.setFillColor(sf::Color::Blue);
    backButtonLabel.setText("Back to menu");
    backButtonLabel.setResizeReference(&backButtonSprite);
    Button backButton(backButtonSprite, [&sceneManager]() { sceneManager.popScene(); });
    GameObject backButtonObject;
    backButtonObject.setPosition(glm::vec2(window.getSize().x / 2.0f, 100));
    backButtonObject.addScriptableBehaviour(backButtonSprite);
    backButtonObject.addScriptableBehaviour(backButtonLabel);
    backButtonObject.addScriptableBehaviour(backButton);


    mainMenu.addChild(playButtonObject);
    mainMenu.addChild(exitButtonObject);
    mainMenu.addChild(fpsTextObject);

    gameScene.addChild(backButtonObject);
    gameScene.addChild(fpsTextObject);

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
