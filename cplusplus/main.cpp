#include "Core.hpp"
#include "Core/GameObjectTemplates.hpp"

#include "Core/Input.hpp"
#include "Core/Scene.hpp"
#include "Core/SceneManager.hpp"

#include "Core/Behaviours/SpriteRenderer.hpp"
#include "Core/Behaviours/TextRenderer.hpp"
#include "Core/Behaviours/Button.hpp"

#include "Game/Behaviours/FPSCounter.hpp"
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

    mainMenu.addScriptableBehaviour<FPSCounter>(0.5f);
    createCharacterScene.addScriptableBehaviour< FPSCounter>(0.5f);

#pragma region Main Menu Scene
    auto [createCharacterButtonObject, cCSprite, cCLabel, cCButton] = DeconstructButton(MakeButton("res/square.png", glm::vec2(200, 40), "Lato", "Create Character", 24,
                                                                                                   TextAlignment::CENTER, sf::Color::Black));
    printf("Created %s at %i\n", createCharacterButtonObject.getIdentifier().c_str(), __LINE__-1);
    createCharacterButtonObject.setPosition(glm::vec2(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 25));
    cCButton.setOnClick([&sceneManager]() { sceneManager.stackScene("createCharacter", true); });

    auto [exitButtonObject, exitSprite, exitLabel, exitButton] = DeconstructButton(MakeButton("res/square.png", glm::vec2(200, 40), "Lato", "Exit", 24, TextAlignment::CENTER,
                                                                                              sf::Color::Black));
    printf("Created %s at %i\n", exitButtonObject.getIdentifier().c_str(), __LINE__-1);
    exitButtonObject.setPosition(glm::vec2(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 25));
    exitButton.setOnClick([&window]() { window.close(); });

    mainMenu.addChild(createCharacterButtonObject);
    mainMenu.addChild(exitButtonObject);
#pragma endregion

#pragma region Create Character Scene
    
    auto windowSize = window.getSize();
    float columnPadding = 10;
    float headerHeight = 40 + 2 * columnPadding;
    sf::FloatRect col1(0, headerHeight, 2.0f * windowSize.x / 5.0f, windowSize.y - headerHeight);
    sf::FloatRect col2(2.0f * windowSize.x / 5.0f, headerHeight, 2.0f * windowSize.x / 5.0f, windowSize.y - headerHeight);
    sf::FloatRect col3(4.0f * windowSize.x / 5.0f, 0, 1.0f * windowSize.x / 5.0f, windowSize.y);

    auto createCharacterObject = MakeEmpty();
    printf("Created %s at %i\n", createCharacterObject.getIdentifier().c_str(), __LINE__-1);
    createCharacterObject.setOrigin(glm::vec2(0));

    auto [createCharTitleObject, cCTLabel] = DeconstructLabel(MakeLabel("Lato", "Create a Character", 32, TextAlignment::CENTER, sf::Color::White));
    printf("Created %s at %i\n", createCharTitleObject.getIdentifier().c_str(), __LINE__-1);
    createCharTitleObject.setPosition(glm::vec2(col1.left + (col1.width + col2.width) / 2.0f, headerHeight / 2.0f));
    createCharacterObject.addChild(createCharTitleObject);

    // col1
    auto [characterNameTextObject, cNLabel] = DeconstructLabel(MakeLabel("Lato", "", 32, TextAlignment::RIGHT, sf::Color::White));
    printf("Created %s at %i\n", characterNameTextObject.getIdentifier().c_str(), __LINE__-1);
    characterNameTextObject.setPosition(glm::vec2(col1.width - 150 - 2 * columnPadding, 2 * columnPadding));
    characterNameTextObject.setOrigin(glm::vec2(0.5f, 0.0f));

    auto [randomizeNameObject, rNSprite, rNLabel, rNButton] = DeconstructButton(MakeButton("res/square.png", glm::vec2(100, 32), "Lato", "Random", 24, TextAlignment::CENTER,
                                                                                           sf::Color::Black));
    printf("Created %s at %i\n", randomizeNameObject.getIdentifier().c_str(), __LINE__-1);
    randomizeNameObject.setPosition(glm::vec2(col1.width - 75 - columnPadding, 2 * columnPadding));

    auto [characterTypeImageObject, cTSprite] = DeconstructSprite(MakeSprite("res/square.png", glm::vec2(0.857142857f * 0.5f * col1.width, 0.5f * col1.width)));
    printf("Created %s at %i\n", characterTypeImageObject.getIdentifier().c_str(), __LINE__-1);
    characterTypeImageObject.setPosition(glm::vec2(col1.width / 2.0f, 2 * columnPadding + cNLabel.getFontSize() + columnPadding));
    characterTypeImageObject.setOrigin(glm::vec2(0.5f, 0.0f));

    auto [characterTypeTextObject, cTTLabel] = DeconstructLabel(MakeLabel("Lato", "", 32, TextAlignment::CENTER, sf::Color::White));
    printf("Created %s at %i\n", characterTypeTextObject.getIdentifier().c_str(), __LINE__-1);
    characterTypeTextObject.setPosition(glm::vec2(col1.width / 2.0f, 3 * columnPadding + 0.5f * col1.width + cNLabel.getFontSize() + 4 * columnPadding));
    characterTypeTextObject.setOrigin(glm::vec2(0.5f, 0.0f));

    auto [decreaseTypeObject, dTSprite, dTLabel, dTButton] = DeconstructButton(
        MakeButton("res/square.png", glm::vec2(32), "Lato", "<", 24, TextAlignment::CENTER, sf::Color::Black));
    printf("Created %s at %i\n", decreaseTypeObject.getIdentifier().c_str(), __LINE__-1);
    decreaseTypeObject.setPosition(glm::vec2(16 + 25 + columnPadding, 3 * columnPadding + 0.5f * col1.width + cNLabel.getFontSize() + 4 * columnPadding));

    auto [increaseTypeObject, iTSprite, iTLabel, iTButton] = DeconstructButton(
        MakeButton("res/square.png", glm::vec2(32), "Lato", ">", 24, TextAlignment::CENTER, sf::Color::Black));
    printf("Created %s at %i\n", increaseTypeObject.getIdentifier().c_str(), __LINE__-1);
    increaseTypeObject.setPosition(glm::vec2(col1.width - 16 - 25 - columnPadding, 3 * columnPadding + 0.5f * col1.width + cNLabel.getFontSize() + 4 * columnPadding));

    auto col1Object = MakeEmpty();
    printf("Created %s at %i\n", col1Object.getIdentifier().c_str(), __LINE__-1);
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
    auto [vitalityPointsObject, vitalityPointsLabel] = DeconstructLabel(MakeLabel("Lato", "<NAME>", 32, TextAlignment::CENTER, sf::Color::White));
    printf("Created %s at %i\n", vitalityPointsObject.getIdentifier().c_str(), __LINE__-1);
    auto [damagePointsObject, damagePointsLabel] = DeconstructLabel(MakeLabel("Lato", "<NAME>", 32, TextAlignment::CENTER, sf::Color::White));
    printf("Created %s at %i\n", damagePointsObject.getIdentifier().c_str(), __LINE__-1);
    auto [defensePointsObject, defensePointsLabel] = DeconstructLabel(MakeLabel("Lato", "<NAME>", 32, TextAlignment::CENTER, sf::Color::White));
    printf("Created %s at %i\n", defensePointsObject.getIdentifier().c_str(), __LINE__-1);
    auto [remainingPointsObject, remainingPointsLabel] = DeconstructLabel(MakeLabel("Lato", "<NAME>", 32, TextAlignment::CENTER, sf::Color::White));
    printf("Created %s at %i\n", remainingPointsObject.getIdentifier().c_str(), __LINE__-1);

    // col3
    auto [summaryLabelObject, sLLabel] = DeconstructLabel(MakeLabel("Lato", "Summary:", 24, TextAlignment::LEFT, sf::Color::White));
    printf("Created %s at %i\n", summaryLabelObject.getIdentifier().c_str(), __LINE__-1);
    summaryLabelObject.setOrigin(glm::vec2(0, 0));
    summaryLabelObject.setPosition(glm::vec2(columnPadding, headerHeight / 2.0f));

    auto [finishCreatingCharacterObject, fCCSprite, fCCLabel, fCCButton] = DeconstructButton(
        MakeButton("res/square.png", glm::vec2(col3.width - 2 * columnPadding, 40), "Lato", "Create", 24, TextAlignment::CENTER, sf::Color::Black));
    printf("Created %s at %i\n", finishCreatingCharacterObject.getIdentifier().c_str(), __LINE__-1);
    finishCreatingCharacterObject.setPosition(glm::vec2(col3.width / 2.0f, col3.height - 30 - 40 - 10));
    fCCButton.setOnClick([&sceneManager]() {
        sceneManager.popScene();
    });
    auto [backFromCharacterCreationObject, bCCSprite, bCCLabel, bCCButton] = DeconstructButton(
        MakeButton("res/square.png", glm::vec2(col3.width - 2 * columnPadding, 40), "Lato", "Back", 24, TextAlignment::CENTER, sf::Color::Black));
    printf("Created %s at %i\n", backFromCharacterCreationObject.getIdentifier().c_str(), __LINE__-1);
    backFromCharacterCreationObject.setPosition(glm::vec2(col3.width / 2.0f, col3.height - 30));
    bCCButton.setOnClick([&sceneManager]() {
        sceneManager.popScene();
    });
    auto col3Object = MakeEmpty();
    printf("Created %s at %i\n", col3Object.getIdentifier().c_str(), __LINE__-1);
    col3Object.setOrigin(glm::vec2(0));
    col3Object.addChild(summaryLabelObject);
    col3Object.addChild(finishCreatingCharacterObject);
    col3Object.addChild(backFromCharacterCreationObject);
    col3Object.setPosition(glm::vec2(col3.left, col3.top));
    createCharacterObject.addChild(col3Object);

    auto& createCharacterController = createCharacterObject.addScriptableBehaviour<CreateCharacterController>(10, &cTSprite, &cNLabel, &cTTLabel, &vitalityPointsLabel, &damagePointsLabel, &defensePointsLabel, &remainingPointsLabel);
    rNButton.setOnClick([&createCharacterController]() {
        createCharacterController.randomizeName();
    });
    dTButton.setOnClick([&createCharacterController]() {
        createCharacterController.changeTypeIndex(-1);
    });
    iTButton.setOnClick([&createCharacterController]() {
        createCharacterController.changeTypeIndex(1);
    });
    

    createCharacterScene.addChild(createCharacterObject);
    
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
