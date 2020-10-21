#include "Core.hpp"

#include "Core/Input.hpp"
#include "Core/Scene.hpp"
#include "Core/SceneManager.hpp"
#include "Game/Character.hpp"

#include "Game/Behaviours/FPSCounter.hpp"
#include "Game/Behaviours/SpriteRenderer.hpp"
#include "Game/Behaviours/TextRenderer.hpp"
#include "Game/Behaviours/Button.hpp"
#include "Game/Behaviours/CreateCharacterController.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Hello SFML");
    window.setVerticalSyncEnabled(false);
    sf::Clock deltaClock;
    srand(time(nullptr));

    Scene createCharacterScene("createCharacter");
    Scene mainMenu("mainMenu");
    SceneManager sceneManager;
    sceneManager.registerScene(createCharacterScene);
    sceneManager.registerScene(mainMenu);
    sceneManager.stackScene(mainMenu, false);

    FPSCounter fpsCounter("fpsCounter", 0.5f);
    GameObject fpsTextObject;
    fpsTextObject.addScriptableBehaviour(fpsCounter);

#pragma region Main Menu Scene
    SpriteRenderer createCharacterButtonSprite("res/square.png");
    createCharacterButtonSprite.setSize(glm::vec2(200, 40));
    TextRenderer createCharacterButtonLabel("Lato");
    createCharacterButtonLabel.setFontSize(24);
    createCharacterButtonLabel.setFillColor(sf::Color::Red);
    createCharacterButtonLabel.setText("Create Character");
    Button createCharacterButton(createCharacterButtonSprite, [&sceneManager]() { sceneManager.stackScene("createCharacter", true); });
    GameObject createCharacterButtonObject;
    createCharacterButtonObject.setPosition(glm::vec2(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 25));
    createCharacterButtonObject.addScriptableBehaviour(createCharacterButtonSprite);
    createCharacterButtonObject.addScriptableBehaviour(createCharacterButtonLabel);
    createCharacterButtonObject.addScriptableBehaviour(createCharacterButton);

    SpriteRenderer exitButtonSprite("res/square.png");
    exitButtonSprite.setSize(glm::vec2(200, 40));
    TextRenderer exitButtonLabel("Lato");
    exitButtonLabel.setFontSize(24);
    exitButtonLabel.setFillColor(sf::Color::Red);
    exitButtonLabel.setText("Exit");
    Button exitButton(exitButtonSprite, [&window]() { window.close(); });
    GameObject exitButtonObject;
    exitButtonObject.setOrigin(glm::vec2(0.5f));
    exitButtonObject.setPosition(glm::vec2(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 25));
    exitButtonObject.addScriptableBehaviour(exitButtonSprite);
    exitButtonObject.addScriptableBehaviour(exitButtonLabel);
    exitButtonObject.addScriptableBehaviour(exitButton);

    mainMenu.addChild(createCharacterButtonObject);
    mainMenu.addChild(exitButtonObject);
    mainMenu.addChild(fpsTextObject);
#pragma endregion

#pragma region Create Character Scene
    auto windowSize = window.getSize();
    float columnPadding = 10;
    float headerHeight = 40 + 2 * columnPadding;
    sf::FloatRect col1(0, headerHeight, 2.0f * windowSize.x / 5.0f, windowSize.y - headerHeight);
    sf::FloatRect col2(2.0f * windowSize.x / 5.0f, headerHeight, 2.0f * windowSize.x / 5.0f, windowSize.y - headerHeight);
    sf::FloatRect col3(4.0f * windowSize.x / 5.0f, 0, 1.0f * windowSize.x / 5.0f, windowSize.y);

    GameObject createCharacterObject;
    createCharacterObject.setOrigin(glm::vec2(0));

    GameObject createCharTitleObject;
    TextRenderer createCharTitle("Lato", "Create a Character", 32, TextAlignment::CENTER, sf::Color::White);
    createCharTitleObject.setPosition(glm::vec2(col1.left + (col1.width + col2.width) / 2.0f, headerHeight / 2.0f));
    createCharTitleObject.addScriptableBehaviour(createCharTitle);

    createCharacterObject.addChild(createCharTitleObject);
    // col1

    GameObject characterNameTextObject;
    TextRenderer characterNameLabel("Lato", "<NAME>", 32, TextAlignment::RIGHT, sf::Color::White);
    characterNameTextObject.setPosition(glm::vec2(col1.width - 150 - 2 * columnPadding, 2 * columnPadding));
    characterNameTextObject.setOrigin(glm::vec2(0.5f, 0.0f));
    characterNameTextObject.addScriptableBehaviour(characterNameLabel);

    GameObject randomizeNameObject;
    SpriteRenderer randomizeNameSprite("res/square.png");
    randomizeNameSprite.setSize(glm::vec2(100, 32));
    TextRenderer randomizeNameLabel("Lato", "Random", 24, TextAlignment::CENTER, sf::Color::Black);
    Button randomizeNameButton(randomizeNameSprite);
    randomizeNameObject.setPosition(glm::vec2(col1.width - 75 - columnPadding, 2 * columnPadding));
    randomizeNameObject.setOrigin(glm::vec2(0.5f, 0.5f));
    randomizeNameObject.addScriptableBehaviour(randomizeNameSprite);
    randomizeNameObject.addScriptableBehaviour(randomizeNameLabel);
    randomizeNameObject.addScriptableBehaviour(randomizeNameButton);

    GameObject characterTypeImageObject;
    SpriteRenderer characterTypeSprite("res/square.png");
    characterTypeSprite.setSize(glm::vec2(0.857142857f * 0.5f * col1.width, 0.5f * col1.width));
    characterTypeImageObject.setPosition(glm::vec2(col1.width / 2.0f, 2 * columnPadding + characterNameLabel.getFontSize() + columnPadding));
    characterTypeImageObject.setOrigin(glm::vec2(0.5f, 0.0f));
    characterTypeImageObject.addScriptableBehaviour(characterTypeSprite);
    GameObject characterTypeTextObject;
    TextRenderer characterTypeLabel("Lato", "<CHARACTER TYPE>", 32, TextAlignment::CENTER, sf::Color::White);
    characterTypeTextObject.setPosition(glm::vec2(col1.width / 2.0f, 3 * columnPadding + 0.5f * col1.width + characterNameLabel.getFontSize() + 4 * columnPadding));
    characterTypeTextObject.setOrigin(glm::vec2(0.5f, 0.0f));
    characterTypeTextObject.addScriptableBehaviour(characterTypeLabel);

    GameObject decreaseTypeObject;
    SpriteRenderer decreaseTypeSprite("res/square.png");
    decreaseTypeSprite.setSize(glm::vec2(32, 32));
    TextRenderer decreaseTypeLabel("Lato", "<", 24, TextAlignment::CENTER, sf::Color::Black);
    Button decreaseTypeButton(decreaseTypeSprite);
    decreaseTypeObject.setPosition(glm::vec2(16 + 25 + columnPadding, 3 * columnPadding + 0.5f * col1.width + characterNameLabel.getFontSize() + 4 * columnPadding));
    decreaseTypeObject.setOrigin(glm::vec2(0.5f, 0.5f));
    decreaseTypeObject.addScriptableBehaviour(decreaseTypeSprite);
    decreaseTypeObject.addScriptableBehaviour(decreaseTypeLabel);
    decreaseTypeObject.addScriptableBehaviour(decreaseTypeButton);

    GameObject increaseTypeObject;
    SpriteRenderer increaseTypeSprite("res/square.png");
    increaseTypeSprite.setSize(glm::vec2(32, 32));
    TextRenderer increaseTypeLabel("Lato", ">", 24, TextAlignment::CENTER, sf::Color::Black);
    Button increaseTypeButton(increaseTypeSprite);
    increaseTypeObject.setPosition(glm::vec2(col1.width - 16 - 25 - columnPadding, 3 * columnPadding + 0.5f * col1.width + characterNameLabel.getFontSize() + 4 * columnPadding));
    increaseTypeObject.setOrigin(glm::vec2(0.5f, 0.5f));
    increaseTypeObject.addScriptableBehaviour(increaseTypeSprite);
    increaseTypeObject.addScriptableBehaviour(increaseTypeLabel);
    increaseTypeObject.addScriptableBehaviour(increaseTypeButton);

    GameObject col1Object;
    col1Object.setOrigin(glm::vec2(0));
    col1Object.addChild(characterNameTextObject);
    col1Object.addChild(randomizeNameObject);
    col1Object.addChild(characterTypeImageObject);
    col1Object.addChild(characterTypeTextObject);
    col1Object.addChild(decreaseTypeObject);
    col1Object.addChild(increaseTypeObject);
    col1Object.setPosition(glm::vec2(col1.left, col1.top));
    createCharacterObject.addChild(col1Object);

    // col2
    TextRenderer vitalityPointsLabel("Lato", "<NAME>", 32, TextAlignment::CENTER, sf::Color::White);
    TextRenderer damagePointsLabel("Lato", "<NAME>", 32, TextAlignment::CENTER, sf::Color::White);
    TextRenderer defensePointsLabel("Lato", "<NAME>", 32, TextAlignment::CENTER, sf::Color::White);
    TextRenderer remainingPointsLabel("Lato", "<NAME>", 32, TextAlignment::CENTER, sf::Color::White);

    // col3
    GameObject summaryLabelObject;
    TextRenderer summaryLabel("Lato", "Summary:", 24, TextAlignment::LEFT, sf::Color::White);
    summaryLabelObject.setOrigin(glm::vec2(0, 0));
    summaryLabelObject.setPosition(glm::vec2(columnPadding, headerHeight / 2.0f));
    summaryLabelObject.addScriptableBehaviour(summaryLabel);

    GameObject createButtonObject;
    SpriteRenderer createButtonSprite("res/square.png");
    createButtonSprite.setSize(glm::vec2(col3.width - 2 * columnPadding, 40));
    TextRenderer createButtonLabel("Lato", "Create", 24, TextAlignment::CENTER, sf::Color::Black);
    Button createButton(createButtonSprite, [&sceneManager]() { sceneManager.popScene(); });
    createButtonObject.setPosition(glm::vec2(col3.width / 2.0f, col3.height - 30 - 40 - 10));
    createButtonObject.addScriptableBehaviour(createButtonSprite);
    createButtonObject.addScriptableBehaviour(createButtonLabel);
    createButtonObject.addScriptableBehaviour(createButton);

    GameObject backButtonObject;
    SpriteRenderer backButtonSprite("res/square.png");
    backButtonSprite.setSize(glm::vec2(col3.width - 2 * columnPadding, 40));
    TextRenderer backButtonLabel("Lato", "Back", 24, TextAlignment::CENTER, sf::Color::Black);
    Button backButton(backButtonSprite, [&sceneManager]() { sceneManager.popScene(); });
    backButtonObject.setPosition(glm::vec2(col3.width / 2.0f, col3.height - 30));
    backButtonObject.addScriptableBehaviour(backButtonSprite);
    backButtonObject.addScriptableBehaviour(backButtonLabel);
    backButtonObject.addScriptableBehaviour(backButton);
    GameObject col3Object;
    col3Object.setOrigin(glm::vec2(0));
    col3Object.addChild(summaryLabelObject);
    col3Object.addChild(createButtonObject);
    col3Object.addChild(backButtonObject);
    col3Object.setPosition(glm::vec2(col3.left, col3.top));
    createCharacterObject.addChild(col3Object);

    CreateCharacterController createCharacterController(10, &characterTypeSprite, &characterNameLabel, &characterTypeLabel, &vitalityPointsLabel, &damagePointsLabel,
                                                        &defensePointsLabel, &remainingPointsLabel);
    createCharacterObject.addScriptableBehaviour(createCharacterController);
    randomizeNameButton.setOnClick([&createCharacterController]() {
        createCharacterController.randomizeName();
    });
    decreaseTypeButton.setOnClick([&createCharacterController]() {
        createCharacterController.changeTypeIndex(-1);
    });
    increaseTypeButton.setOnClick([&createCharacterController]() {
        createCharacterController.changeTypeIndex(1);
    });

    createCharacterScene.addChild(createCharacterObject);
    createCharacterScene.addChild(fpsTextObject);
#pragma endregion

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
