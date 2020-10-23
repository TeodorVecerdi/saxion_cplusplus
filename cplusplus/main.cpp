#include "Core.hpp"
#include "Core/GameObjectTemplates.hpp"

#include "Core/Input.hpp"
#include "Core/Scene.hpp"
#include "Core/SceneManager.hpp"

#include "Core/Behaviours/SpriteRenderer.hpp"
#include "Core/Behaviours/TextRenderer.hpp"
#include "Core/Behaviours/Button.hpp"
#include "Game/Character.hpp"

#include "Game/Behaviours/FPSCounter.hpp"
#include "Game/Behaviours/CreateCharacterController.hpp"
#include "Game/Behaviours/PlayerController.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Hello SFML");
    window.setVerticalSyncEnabled(false);
    sf::Clock deltaClock;
    srand(time(nullptr));

    Scene createCharacterScene("createCharacter");
    Scene mainMenu("mainMenu");
    Scene gameScene("game");
    SceneManager sceneManager;
    sceneManager.registerScene(createCharacterScene);
    sceneManager.registerScene(mainMenu);
    sceneManager.registerScene(gameScene);
    sceneManager.stackScene(mainMenu, false);

    mainMenu.addScriptableBehaviour<FPSCounter>(0.5f);
    createCharacterScene.addScriptableBehaviour<FPSCounter>(0.5f);

    Character* player = new Character();

#pragma region Main Menu Scene
    auto [createCharacterButtonObject, cCSprite, cCLabel, cCButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(200, 40), "Lato", "Play", 24,
                                                                                                   TextAlignment::CENTER, sf::Color::Black));
    createCharacterButtonObject.setPosition(glm::vec2(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 25));
    cCButton.setOnClick([&sceneManager]() { sceneManager.stackScene("createCharacter", true); });

    auto [exitButtonObject, exitSprite, exitLabel, exitButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(200, 40), "Lato", "Exit", 24,
                                                                                              TextAlignment::CENTER, sf::Color::Black));
    exitButtonObject.setPosition(glm::vec2(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 25));
    exitButton.setOnClick([&window]() { window.close(); });

    mainMenu.addChild(createCharacterButtonObject);
    mainMenu.addChild(exitButtonObject);
#pragma endregion

#pragma region Create Character Scene

    auto windowSize = window.getSize();
    float columnPadding = 10;
    float headerHeight = 48 + 2 * columnPadding;
    sf::FloatRect col1(0, headerHeight, 3.0f * windowSize.x / 7.0f, windowSize.y - headerHeight);
    sf::FloatRect col2(3.0f * windowSize.x / 7.0f, headerHeight, 3.0f * windowSize.x / 7.0f, windowSize.y - headerHeight);
    sf::FloatRect col3(6.0f * windowSize.x / 7.0f, 0, 1.0f * windowSize.x / 7.0f, windowSize.y);

    auto& createCharacterObject = MakeEmpty();
    createCharacterObject.setOrigin(glm::vec2(0));

    auto [createCharTitleObject, cCTLabel] = DeconstructLabel(MakeLabel("Lato", "Create a Character", 48, TextAlignment::CENTER, sf::Color::White));
    createCharTitleObject.setPosition(glm::vec2(col1.left + windowSize.x / 2.0f, headerHeight / 2.0f));
    createCharacterObject.addChild(createCharTitleObject);

    // col1
    auto [characterNameTextObject, cNLabel] = DeconstructLabel(MakeLabel("Lato", "", 32, TextAlignment::RIGHT, sf::Color::White));
    characterNameTextObject.setPosition(glm::vec2(col1.width - 150 - 2 * columnPadding, 2 * columnPadding));
    characterNameTextObject.setOrigin(glm::vec2(0.5f, 0.0f));

    auto [randomizeNameObject, rNSprite, rNLabel, rNButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(100, 32), "Lato", "Random", 24,
                                                                                           TextAlignment::CENTER, sf::Color::Black));
    randomizeNameObject.setPosition(glm::vec2(col1.width - 75 - columnPadding, 2 * columnPadding));

    auto [characterTypeImageObject, cTSprite] = DeconstructSprite(MakeSprite("assets/textures/square.png", glm::vec2(0.857142857f * 0.5f * col1.width, 0.5f * col1.width)));
    characterTypeImageObject.setPosition(glm::vec2(col1.width / 2.0f, 2 * columnPadding + cNLabel.getFontSize() + columnPadding));
    characterTypeImageObject.setOrigin(glm::vec2(0.5f, 0.0f));

    auto [characterTypeTextObject, cTTLabel] = DeconstructLabel(MakeLabel("Lato", "", 32, TextAlignment::CENTER, sf::Color::White));
    characterTypeTextObject.setPosition(glm::vec2(col1.width / 2.0f, 3 * columnPadding + 0.5f * col1.width + cNLabel.getFontSize() + 4 * columnPadding));
    characterTypeTextObject.setOrigin(glm::vec2(0.5f, 0.0f));

    auto [decreaseTypeObject, dTSprite, dTLabel, dTButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(32), "Lato", "<", 24, TextAlignment::CENTER,
                                                                                          sf::Color::Black));
    decreaseTypeObject.setPosition(glm::vec2(16 + 25 + columnPadding, 3 * columnPadding + 0.5f * col1.width + cNLabel.getFontSize() + 4 * columnPadding));

    auto [increaseTypeObject, iTSprite, iTLabel, iTButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(32), "Lato", ">", 24, TextAlignment::CENTER,
                                                                                          sf::Color::Black));
    increaseTypeObject.setPosition(glm::vec2(col1.width - 16 - 25 - columnPadding, 3 * columnPadding + 0.5f * col1.width + cNLabel.getFontSize() + 4 * columnPadding));

    auto& col1Object = MakeEmpty();
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
    float lineHeight32 = 32.0f;
    auto [remainingPointsObject, remainingPointsLabel] = DeconstructLabel(MakeLabel("Lato", "<NAME>", 32, TextAlignment::LEFT, sf::Color::White));
    remainingPointsObject.setPosition(glm::vec2(2 * columnPadding, 2 * columnPadding));

    auto [vitalityPointsObject, vitalityPointsLabel] = DeconstructLabel(MakeLabel("Lato", "<NAME>", 32, TextAlignment::LEFT, sf::Color::White));
    auto [vitalityIncreaseObject, vISprite, vILabel, vIButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(32), "Lato", "+", 32, TextAlignment::CENTER,
                                                                                              sf::Color::Black));
    auto [vitalityDecreaseObject, vDSprite, vDLabel, vDButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(32), "Lato", "-", 32, TextAlignment::CENTER,
                                                                                              sf::Color::Black));
    vitalityPointsObject.setPosition(glm::vec2(2.0f * columnPadding, lineHeight32 + 3.0f * columnPadding));
    vitalityIncreaseObject.setPosition(glm::vec2(col2.width - 16 - 2.0f * columnPadding, lineHeight32 + 3.0f * columnPadding));
    vitalityDecreaseObject.setPosition(glm::vec2(col2.width - 48 - 3.0f * columnPadding, lineHeight32 + 3.0f * columnPadding));

    auto [damagePointsObject, damagePointsLabel] = DeconstructLabel(MakeLabel("Lato", "<NAME>", 32, TextAlignment::LEFT, sf::Color::White));
    auto [damageIncreaseObject, dmgISprite, dmgILabel, dmgIButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(32), "Lato", "+", 32,
                                                                                                  TextAlignment::CENTER, sf::Color::Black));
    auto [damageDecreaseObject, dmgDSprite, dmgDLabel, dmgDButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(32), "Lato", "-", 32,
                                                                                                  TextAlignment::CENTER, sf::Color::Black));
    damagePointsObject.setPosition(glm::vec2(2.0f * columnPadding, 2.0f * lineHeight32 + 3.5f * columnPadding));
    damageIncreaseObject.setPosition(glm::vec2(col2.width - 16 - 2.0f * columnPadding, 2.0f * lineHeight32 + 3.5f * columnPadding));
    damageDecreaseObject.setPosition(glm::vec2(col2.width - 48 - 3.0f * columnPadding, 2.0f * lineHeight32 + 3.5f * columnPadding));

    auto [defensePointsObject, defensePointsLabel] = DeconstructLabel(MakeLabel("Lato", "<NAME>", 32, TextAlignment::LEFT, sf::Color::White));
    auto [defenseIncreaseObject, dfISprite, dfILabel, dfIButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(32), "Lato", "+", 32, TextAlignment::CENTER,
                                                                                                sf::Color::Black));
    auto [defenseDecreaseObject, dfDSprite, dfDLabel, dfDButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(32), "Lato", "-", 32, TextAlignment::CENTER,
                                                                                                sf::Color::Black));
    defensePointsObject.setPosition(glm::vec2(2.0f * columnPadding, 3.0f * lineHeight32 + 4.0f * columnPadding));
    defenseIncreaseObject.setPosition(glm::vec2(col2.width - 16 - 2.0f * columnPadding, 3.0f * lineHeight32 + 4.0f * columnPadding));
    defenseDecreaseObject.setPosition(glm::vec2(col2.width - 48 - 3.0f * columnPadding, 3.0f * lineHeight32 + 4.0f * columnPadding));

    auto [staminaPointsObject, staminaPointsLabel] = DeconstructLabel(MakeLabel("Lato", "<NAME>", 32, TextAlignment::LEFT, sf::Color::White));
    auto [staminaIncreaseObject, stISprite, stILabel, stIButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(32), "Lato", "+", 32, TextAlignment::CENTER,
                                                                                                sf::Color::Black));
    auto [staminaDecreaseObject, stDSprite, stDLabel, stDButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(32), "Lato", "-", 32, TextAlignment::CENTER,
                                                                                                sf::Color::Black));
    staminaPointsObject.setPosition(glm::vec2(2.0f * columnPadding, 4.0f * lineHeight32 + 4.5f * columnPadding));
    staminaIncreaseObject.setPosition(glm::vec2(col2.width - 16 - 2.0f * columnPadding, 4.0f * lineHeight32 + 4.5f * columnPadding));
    staminaDecreaseObject.setPosition(glm::vec2(col2.width - 48 - 3.0f * columnPadding, 4.0f * lineHeight32 + 4.5f * columnPadding));

    auto& col2Object = MakeEmpty();
    col2Object.setOrigin(glm::vec2(0));
    col2Object.setPosition(glm::vec2(col2.left, col2.top));
    col2Object.addChild(remainingPointsObject);
    col2Object.addChild(vitalityPointsObject);
    col2Object.addChild(vitalityIncreaseObject);
    col2Object.addChild(vitalityDecreaseObject);
    col2Object.addChild(damagePointsObject);
    col2Object.addChild(damageIncreaseObject);
    col2Object.addChild(damageDecreaseObject);
    col2Object.addChild(defensePointsObject);
    col2Object.addChild(defenseIncreaseObject);
    col2Object.addChild(defenseDecreaseObject);
    col2Object.addChild(staminaPointsObject);
    col2Object.addChild(staminaIncreaseObject);
    col2Object.addChild(staminaDecreaseObject);

    createCharacterObject.addChild(col2Object);

    // col3

    auto [finishCreatingCharacterObject, fCCSprite, fCCLabel, fCCButton] = DeconstructButton(
        MakeButton("assets/textures/square.png", glm::vec2(col3.width - 2 * columnPadding, 40), "Lato", "Create", 24, TextAlignment::CENTER, sf::Color::Black));
    finishCreatingCharacterObject.setPosition(glm::vec2(col3.width / 2.0f, col3.height - 30 - 40 - 10));

    auto [backFromCharacterCreationObject, bCCSprite, bCCLabel, bCCButton] = DeconstructButton(
        MakeButton("assets/textures/square.png", glm::vec2(col3.width - 2 * columnPadding, 40), "Lato", "Back", 24, TextAlignment::CENTER, sf::Color::Black));
    backFromCharacterCreationObject.setPosition(glm::vec2(col3.width / 2.0f, col3.height - 30));

    auto& col3Object = MakeEmpty();
    col3Object.setOrigin(glm::vec2(0));
    col3Object.addChild(finishCreatingCharacterObject);
    col3Object.addChild(backFromCharacterCreationObject);
    col3Object.setPosition(glm::vec2(col3.left, col3.top));
    createCharacterObject.addChild(col3Object);

    auto& createCharacterController = createCharacterObject.addScriptableBehaviour<CreateCharacterController>(
        10, &cTSprite, &cNLabel, &cTTLabel, &vitalityPointsLabel, &damagePointsLabel, &defensePointsLabel, &staminaPointsLabel, &remainingPointsLabel);

    fCCButton.setOnClick([&sceneManager, &createCharacterController, &player]() {
        createCharacterController.exportCharacter();
        player->loadFromFile(createCharacterController.getExportPath());
        sceneManager.popScene();
        sceneManager.stackScene("game", false);
    });
    bCCButton.setOnClick([&sceneManager]() {
        sceneManager.popScene();
    });
    rNButton.setOnClick([&createCharacterController]() {
        createCharacterController.randomizeName();
    });
    dTButton.setOnClick([&createCharacterController]() {
        createCharacterController.changeTypeIndex(-1);
    });
    iTButton.setOnClick([&createCharacterController]() {
        createCharacterController.changeTypeIndex(1);
    });
    vDButton.setOnClick([&createCharacterController]() {
        createCharacterController.changeVitality(-1);
    });
    vIButton.setOnClick([&createCharacterController]() {
        createCharacterController.changeVitality(1);
    });
    dmgDButton.setOnClick([&createCharacterController]() {
        createCharacterController.changeDamage(-1);
    });
    dmgIButton.setOnClick([&createCharacterController]() {
        createCharacterController.changeDamage(1);
    });

    dfDButton.setOnClick([&createCharacterController]() {
        createCharacterController.changeDefense(-1);
    });
    dfIButton.setOnClick([&createCharacterController]() {
        createCharacterController.changeDefense(1);
    });
    stDButton.setOnClick([&createCharacterController]() {
        createCharacterController.changeStamina(-1);
    });
    stIButton.setOnClick([&createCharacterController]() {
        createCharacterController.changeStamina(1);
    });

    createCharacterScene.addChild(createCharacterObject);

#pragma endregion

#pragma region Game Scene
    auto& gameContainer = MakeEmpty();
    auto [playerImageObject, pISprite] = DeconstructSprite(MakeSprite("assets/textures/square.png", glm::vec2(512, 512.0f * (54.0f / 48.0f))));
    playerImageObject.setPosition(glm::vec2(256.0f + 4*columnPadding, windowSize.y / 2.0f));
    auto [setIdleStateObject, sISSprite, sISLabel, sISButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(200, 40), "Lato", "Set Idle", 32,
                                                                                             TextAlignment::CENTER, sf::Color::Black));
    setIdleStateObject.setPosition(glm::vec2(windowSize.x - 100 - columnPadding, windowSize.y / 2.0f - 40 - columnPadding));
    auto [setHurtStateObject, sHSSprite, sHSLabel, sHSButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(200, 40), "Lato", "Set Hurt", 32,
                                                                                             TextAlignment::CENTER, sf::Color::Black));
    setHurtStateObject.setPosition(glm::vec2(windowSize.x - 100 - columnPadding, windowSize.y / 2.0f));
    auto [setAttackStateObject, sASSprite, sASLabel, sASButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(200, 40), "Lato", "Set Attack", 32,
                                                                                               TextAlignment::CENTER, sf::Color::Black));
    setAttackStateObject.setPosition(glm::vec2(windowSize.x - 100 - columnPadding, windowSize.y / 2.0f + 40 + columnPadding));

    auto& playerController = gameContainer.addScriptableBehaviour<PlayerController>(player, pISprite);
    sISButton.setOnClick([&playerController]() {
        playerController.setIdleState();
    });

    sHSButton.setOnClick([&playerController]() {
        playerController.setHurtState();
    });

    sASButton.setOnClick([&playerController]() {
        playerController.setAttackState();
    });
    gameContainer.addChild(playerImageObject);
    gameContainer.addChild(setIdleStateObject);
    gameContainer.addChild(setHurtStateObject);
    gameContainer.addChild(setAttackStateObject);
    gameScene.addChild(gameContainer);
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
