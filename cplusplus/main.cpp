#include <filesystem>
#include "Core.hpp"
#include "Core/Input.hpp"

#include "Core/Scene/Scene.hpp"
#include "Core/Scene/SceneManager.hpp"
#include "Core/Scene/GameObjectTemplates.hpp"

#include "Core/Behaviours/SpriteRenderer.hpp"
#include "Core/Behaviours/TextRenderer.hpp"
#include "Core/Behaviours/Button.hpp"
#include "Core/Behaviours/FilledSpriteRenderer.hpp"

#include "Game/Behaviours/FPSCounter.hpp"
#include "Game/Behaviours/CreateCharacterController.hpp"
#include "Game/Behaviours/PlayerController.hpp"
#include "Game/Character.hpp"
#include "Game/Behaviours/TestBehaviour.h"

int main() {
    // Clean temp folder
    std::filesystem::path temp("assets/temp");
    std::filesystem::remove_all(temp);
    std::filesystem::create_directory(temp);

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
    auto* player = new Character();

    // Colors
    sf::Color lavenderBlush(234, 245, 255);
    sf::Color mintCream(246, 255, 255);
    sf::Color imperialRed(233, 54, 64);
    sf::Color pacificBlue(15, 163, 184);
    sf::Color oxfordBlue(0, 0, 25);
    sf::Color independance(73, 71, 91);
    sf::Color blackCoral(94, 105, 115);
    sf::Color raisinBlack(32, 32, 48);

    // Theme
    sf::Color primaryText = mintCream;
    sf::Color secondaryText = oxfordBlue;
    sf::Color background = oxfordBlue;
    sf::Color panelDark = raisinBlack;
    sf::Color panelLight = independance;
    sf::Color themeColor1 = imperialRed;
    sf::Color themeColor2 = pacificBlue;

#pragma region Main Menu Scene
    auto [createCharacterButtonObject, cCSprite, cCLabel, cCButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(200, 40), "Lato", "Play", 24,
        TextAlignment::CENTER, secondaryText));
    createCharacterButtonObject.setPosition(glm::vec2(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 25));
    cCButton.setOnClick([&sceneManager]() { sceneManager.stackScene("createCharacter", true); });

    auto [exitButtonObject, exitSprite, exitLabel, exitButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(200, 40), "Lato", "Exit", 24,
        TextAlignment::CENTER, secondaryText));
    exitButtonObject.setPosition(glm::vec2(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 25));
    exitButton.setOnClick([&window]() { window.close(); });

    auto [fillTest, fillSprite] = DeconstructSprite(MakeSprite("assets/textures/square.png", glm::vec2(200, 50)));
    fillTest.setPosition(glm::vec2(window.getSize().x / 2.0f, 65));
    fillTest.setOrigin(glm::vec2(0, 0.5f));
    fillTest.addScriptableBehaviour<TestBehaviour>(fillSprite);
    fillSprite.setColor(themeColor1);

    mainMenu.addChild(fillTest);
    mainMenu.addChild(createCharacterButtonObject);
    mainMenu.addChild(exitButtonObject);
#pragma endregion

#pragma region Create Character Scene

    auto windowSize = window.getSize();
    float UIPadding = 10;
    float headerHeight = 48 + 2 * UIPadding;
    sf::FloatRect col1(0, headerHeight, 3.0f * windowSize.x / 7.0f, windowSize.y - headerHeight);
    sf::FloatRect col2(3.0f * windowSize.x / 7.0f, headerHeight, 3.0f * windowSize.x / 7.0f, windowSize.y - headerHeight);
    sf::FloatRect col3(6.0f * windowSize.x / 7.0f, 0, 1.0f * windowSize.x / 7.0f, windowSize.y);

    auto& createCharacterObject = MakeEmpty();
    createCharacterObject.setOrigin(glm::vec2(0));

    auto [createCharTitleObject, cCTLabel] = DeconstructLabel(MakeLabel("Lato", "Create a Character", 48, TextAlignment::CENTER, primaryText));
    createCharTitleObject.setPosition(glm::vec2(col1.left + windowSize.x / 2.0f, headerHeight / 2.0f));
    createCharacterObject.addChild(createCharTitleObject);

    // col1
    auto [characterNameTextObject, cNLabel] = DeconstructLabel(MakeLabel("Lato", "", 32, TextAlignment::RIGHT, primaryText));
    characterNameTextObject.setPosition(glm::vec2(col1.width - 150 - 2 * UIPadding, 2 * UIPadding));
    characterNameTextObject.setOrigin(glm::vec2(0.5f, 0.0f));

    auto [randomizeNameObject, rNSprite, rNLabel, rNButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(100, 32), "Lato", "Random", 24,
        TextAlignment::CENTER, secondaryText));
    randomizeNameObject.setPosition(glm::vec2(col1.width - 75 - UIPadding, 2 * UIPadding));

    auto [characterTypeImageObject, cTSprite] = DeconstructSprite(MakeSprite("assets/textures/square.png", glm::vec2(0.857142857f * 0.5f * col1.width, 0.5f * col1.width)));
    characterTypeImageObject.setPosition(glm::vec2(col1.width / 2.0f, 2 * UIPadding + cNLabel.getFontSize() + UIPadding));
    characterTypeImageObject.setOrigin(glm::vec2(0.5f, 0.0f));

    auto [characterTypeTextObject, cTTLabel] = DeconstructLabel(MakeLabel("Lato", "", 32, TextAlignment::CENTER, primaryText));
    characterTypeTextObject.setPosition(glm::vec2(col1.width / 2.0f, 3 * UIPadding + 0.5f * col1.width + cNLabel.getFontSize() + 4 * UIPadding));
    characterTypeTextObject.setOrigin(glm::vec2(0.5f, 0.0f));

    auto [decreaseTypeObject, dTSprite, dTLabel, dTButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(32), "Lato", "<", 24, TextAlignment::CENTER,
        secondaryText));
    decreaseTypeObject.setPosition(glm::vec2(16 + 25 + UIPadding, 3 * UIPadding + 0.5f * col1.width + cNLabel.getFontSize() + 4 * UIPadding));

    auto [increaseTypeObject, iTSprite, iTLabel, iTButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(32), "Lato", ">", 24, TextAlignment::CENTER,
        secondaryText));
    increaseTypeObject.setPosition(glm::vec2(col1.width - 16 - 25 - UIPadding, 3 * UIPadding + 0.5f * col1.width + cNLabel.getFontSize() + 4 * UIPadding));

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
    auto [remainingPointsObject, remainingPointsLabel] = DeconstructLabel(MakeLabel("Lato", "<NAME>", 32, TextAlignment::LEFT, primaryText));
    remainingPointsObject.setPosition(glm::vec2(2 * UIPadding, 2 * UIPadding));

    auto [vitalityPointsObject, vitalityPointsLabel] = DeconstructLabel(MakeLabel("Lato", "<NAME>", 32, TextAlignment::LEFT, primaryText));
    auto [vitalityIncreaseObject, vISprite, vILabel, vIButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(32), "Lato", "+", 32, TextAlignment::CENTER,
        secondaryText));
    auto [vitalityDecreaseObject, vDSprite, vDLabel, vDButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(32), "Lato", "-", 32, TextAlignment::CENTER,
        secondaryText));
    vitalityPointsObject.setPosition(glm::vec2(2.0f * UIPadding, lineHeight32 + 3.0f * UIPadding));
    vitalityIncreaseObject.setPosition(glm::vec2(col2.width - 16 - 2.0f * UIPadding, lineHeight32 + 3.0f * UIPadding));
    vitalityDecreaseObject.setPosition(glm::vec2(col2.width - 48 - 3.0f * UIPadding, lineHeight32 + 3.0f * UIPadding));

    auto [damagePointsObject, damagePointsLabel] = DeconstructLabel(MakeLabel("Lato", "<NAME>", 32, TextAlignment::LEFT, primaryText));
    auto [damageIncreaseObject, dmgISprite, dmgILabel, dmgIButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(32), "Lato", "+", 32,
        TextAlignment::CENTER, secondaryText));
    auto [damageDecreaseObject, dmgDSprite, dmgDLabel, dmgDButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(32), "Lato", "-", 32,
        TextAlignment::CENTER, secondaryText));
    damagePointsObject.setPosition(glm::vec2(2.0f * UIPadding, 2.0f * lineHeight32 + 3.5f * UIPadding));
    damageIncreaseObject.setPosition(glm::vec2(col2.width - 16 - 2.0f * UIPadding, 2.0f * lineHeight32 + 3.5f * UIPadding));
    damageDecreaseObject.setPosition(glm::vec2(col2.width - 48 - 3.0f * UIPadding, 2.0f * lineHeight32 + 3.5f * UIPadding));

    auto [defensePointsObject, defensePointsLabel] = DeconstructLabel(MakeLabel("Lato", "<NAME>", 32, TextAlignment::LEFT, primaryText));
    auto [defenseIncreaseObject, dfISprite, dfILabel, dfIButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(32), "Lato", "+", 32, TextAlignment::CENTER,
        secondaryText));
    auto [defenseDecreaseObject, dfDSprite, dfDLabel, dfDButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(32), "Lato", "-", 32, TextAlignment::CENTER,
        secondaryText));
    defensePointsObject.setPosition(glm::vec2(2.0f * UIPadding, 3.0f * lineHeight32 + 4.0f * UIPadding));
    defenseIncreaseObject.setPosition(glm::vec2(col2.width - 16 - 2.0f * UIPadding, 3.0f * lineHeight32 + 4.0f * UIPadding));
    defenseDecreaseObject.setPosition(glm::vec2(col2.width - 48 - 3.0f * UIPadding, 3.0f * lineHeight32 + 4.0f * UIPadding));

    auto [staminaPointsObject, staminaPointsLabel] = DeconstructLabel(MakeLabel("Lato", "<NAME>", 32, TextAlignment::LEFT, primaryText));
    auto [staminaIncreaseObject, stISprite, stILabel, stIButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(32), "Lato", "+", 32, TextAlignment::CENTER,
        secondaryText));
    auto [staminaDecreaseObject, stDSprite, stDLabel, stDButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(32), "Lato", "-", 32, TextAlignment::CENTER,
        secondaryText));
    staminaPointsObject.setPosition(glm::vec2(2.0f * UIPadding, 4.0f * lineHeight32 + 4.5f * UIPadding));
    staminaIncreaseObject.setPosition(glm::vec2(col2.width - 16 - 2.0f * UIPadding, 4.0f * lineHeight32 + 4.5f * UIPadding));
    staminaDecreaseObject.setPosition(glm::vec2(col2.width - 48 - 3.0f * UIPadding, 4.0f * lineHeight32 + 4.5f * UIPadding));

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
        MakeButton("assets/textures/square.png", glm::vec2(col3.width - 2 * UIPadding, 40), "Lato", "Create", 24, TextAlignment::CENTER, secondaryText));
    finishCreatingCharacterObject.setPosition(glm::vec2(col3.width / 2.0f, col3.height - 30 - 40 - 10));

    auto [backFromCharacterCreationObject, bCCSprite, bCCLabel, bCCButton] = DeconstructButton(
        MakeButton("assets/textures/square.png", glm::vec2(col3.width - 2 * UIPadding, 40), "Lato", "Back", 24, TextAlignment::CENTER, secondaryText));
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

    auto characterContainerSize = glm::vec2(windowSize.x / 2.0f - 12 * UIPadding, windowSize.y / 2.0f - 12 * UIPadding);
    

    auto playerImageSize = glm::vec2(1.8f / 5.0f * characterContainerSize.x, 1.8f / 5.0f * characterContainerSize.x * (54.0f / 48.0f));
    auto [playerImageObject, playerImageSprite] = DeconstructSprite(MakeSprite("assets/textures/square.png", playerImageSize));
    playerImageObject.setPosition(glm::vec2(-characterContainerSize.x / 2.0f + playerImageSize.x / 2.0f + UIPadding, 0));

    auto& playerHealth = MakeEmpty();
    playerHealth.setPosition(glm::vec2(-(characterContainerSize.x / 2.0f - playerImageSize.x - 3 * UIPadding), -characterContainerSize.y / 2 + 20.0f + UIPadding));
    playerHealth.setOrigin(glm::vec2(0.0f, 0.5f));
    auto& playerHealthSprite = playerHealth.addScriptableBehaviour<FilledSpriteRenderer>("assets/textures/square.png", false);
    playerHealthSprite.setSize(glm::vec2(characterContainerSize.x - playerImageSize.x - 4 * UIPadding, 40));
    playerHealthSprite.setColor(themeColor1);
    playerHealthSprite.setFillAmount(1);
    auto [playerHealthLabel1, playerHealthLabel1Text] = DeconstructLabel(MakeLabel("Lato", "Health:", 24, TextAlignment::LEFT, primaryText));
    playerHealthLabel1.setPosition(glm::vec2(UIPadding, 0));
    auto [playerHealthLabel2, playerHealthLabel2Text] = DeconstructLabel(MakeLabel("Lato", "##/##", 24, TextAlignment::RIGHT, primaryText));
    playerHealthLabel2.setPosition(glm::vec2(characterContainerSize.x / 2.0f + playerImageSize.x / 2.0f - (4 + 2 + 1) * UIPadding, 0));
    playerHealth.addChild(playerHealthLabel1);
    playerHealth.addChild(playerHealthLabel2);
    

    // Todo: implement experience/levels
    auto& playerLevel = MakeEmpty();
    playerLevel.setPosition(glm::vec2(-(characterContainerSize.x / 2.0f - playerImageSize.x - 3 * UIPadding), -characterContainerSize.y / 2 + 20.0f + 40.0f + 1.5f * UIPadding));
    playerLevel.setOrigin(glm::vec2(0.0f, 0.5f));
    auto& playerLevelSprite = playerLevel.addScriptableBehaviour<FilledSpriteRenderer>("assets/textures/square.png", false);
    playerLevelSprite.setSize(glm::vec2(characterContainerSize.x - playerImageSize.x - 4 * UIPadding, 40));
    playerLevelSprite.setColor(themeColor2);
    playerLevelSprite.setFillAmount(1);
    auto [playerLevelLabel1, playerLevelLabel1Text] = DeconstructLabel(MakeLabel("Lato", "Level: ##", 24, TextAlignment::LEFT, primaryText));
    playerLevelLabel1.setPosition(glm::vec2(UIPadding, 0));
    auto [playerLevelLabel2, playerLevelLabel2Text] = DeconstructLabel(MakeLabel("Lato", "###/###", 24, TextAlignment::RIGHT, primaryText));
    playerLevelLabel2.setPosition(glm::vec2(characterContainerSize.x / 2.0f + playerImageSize.x / 2.0f - (4 + 2 + 1) * UIPadding, 0));
    playerLevel.addChild(playerLevelLabel1);
    playerLevel.addChild(playerLevelLabel2);
    
    auto [playerName, playerNameLabel] = DeconstructLabel(MakeLabel("Lato", "<CharacterName>", 32, TextAlignment::LEFT, primaryText));
    playerName.setPosition(glm::vec2(-characterContainerSize.x / 2.0f, -characterContainerSize.y / 2.0f - 16 - 0.5f * UIPadding));
    
    auto [playerStatus, playerStatusLabel] = DeconstructLabel(MakeLabel("Lato", "...<Status>", 32, TextAlignment::RIGHT, primaryText));
    playerStatus.setPosition(glm::vec2(characterContainerSize.x / 2.0f, -characterContainerSize.y / 2.0f - 16 - 0.5f * UIPadding));
    
    const float buttonHeight = characterContainerSize.y / 2.0f - 40 - 2.25f * UIPadding;

    auto [playerAttack, playerAttackSprite, playerAttackLabel, playerAttackButton]
        = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(characterContainerSize.x - playerImageSize.x - 4 * UIPadding, buttonHeight), "Lato", "ATTACK", 24, TextAlignment::CENTER, secondaryText)); // todo create button
    playerAttack.setOrigin(glm::vec2(0.5f, 0.5f));
    playerAttack.setPosition(glm::vec2(playerImageSize.x / 2.0f + UIPadding, 2.0f * UIPadding));
    
    auto [playerFocus, playerFocusSprite, playerFocusLabel, playerFocusButton]
        = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(characterContainerSize.x - playerImageSize.x - 4 * UIPadding, buttonHeight), "Lato", "FOCUS", 24, TextAlignment::CENTER, secondaryText)); // todo create button
    playerFocus.setOrigin(glm::vec2(0.5f, 0.5f));
    playerFocus.setPosition(glm::vec2(playerImageSize.x / 2.0f + UIPadding, buttonHeight + 2.5f * UIPadding));
    
    auto [playerContainer, playerContainerBackground] = DeconstructSprite(MakeSprite("assets/textures/square.png", characterContainerSize));
    playerContainerBackground.setColor(panelLight);
    playerContainer.setPosition(glm::vec2(characterContainerSize.x / 2.0f + 4.0f * UIPadding, windowSize.y / 2.0f - characterContainerSize.y/2.0f));
    playerContainer.addChild(playerImageObject);
    playerContainer.addChild(playerHealth);
    playerContainer.addChild(playerLevel);
    playerContainer.addChild(playerName);
    playerContainer.addChild(playerStatus);
    playerContainer.addChild(playerAttack);
    playerContainer.addChild(playerFocus);

    auto enemyImageSize = glm::vec2(1.8f / 5.0f * characterContainerSize.x, 1.8f / 5.0f * characterContainerSize.x * (54.0f / 48.0f));
    auto [enemyImageObject, enemyImageSprite] = DeconstructSprite(MakeSprite("assets/textures/square.png", enemyImageSize));
    enemyImageObject.setPosition(glm::vec2(-characterContainerSize.x / 2.0f + enemyImageSize.x / 2.0f + UIPadding, 0));

    auto& enemyHealth = MakeEmpty();
    enemyHealth.setPosition(glm::vec2(-(characterContainerSize.x / 2.0f - enemyImageSize.x - 3 * UIPadding), -characterContainerSize.y / 2 + 20.0f + UIPadding));
    enemyHealth.setOrigin(glm::vec2(0.0f, 0.5f));
    auto& enemyHealthSprite = enemyHealth.addScriptableBehaviour<FilledSpriteRenderer>("assets/textures/square.png", false);
    enemyHealthSprite.setSize(glm::vec2(characterContainerSize.x - enemyImageSize.x - 4 * UIPadding, 40));
    enemyHealthSprite.setColor(themeColor1);
    enemyHealthSprite.setFillAmount(1);
    auto [enemyHealthLabel1, enemyHealthLabel1Text] = DeconstructLabel(MakeLabel("Lato", "Health:", 24, TextAlignment::LEFT, primaryText));
    enemyHealthLabel1.setPosition(glm::vec2(UIPadding, 0));
    auto [enemyHealthLabel2, enemyHealthLabel2Text] = DeconstructLabel(MakeLabel("Lato", "##/##", 24, TextAlignment::RIGHT, primaryText));
    enemyHealthLabel2.setPosition(glm::vec2(characterContainerSize.x / 2.0f + enemyImageSize.x / 2.0f - (4 + 2 + 1) * UIPadding, 0));
    enemyHealth.addChild(enemyHealthLabel1);
    enemyHealth.addChild(enemyHealthLabel2);

    auto [enemyStatus, enemyStatusLabel] = DeconstructLabel(MakeLabel("Lato", "<Status>...", 32, TextAlignment::LEFT, primaryText));
    enemyStatus.setPosition(glm::vec2(-characterContainerSize.x / 2.0f, -characterContainerSize.y / 2.0f - 16 - 0.5f * UIPadding));

    auto [enemyName, enemyNameLabel] = DeconstructLabel(MakeLabel("Lato", "<EnemyName>", 32, TextAlignment::RIGHT, primaryText));
    enemyName.setPosition(glm::vec2(characterContainerSize.x / 2.0f, -characterContainerSize.y / 2.0f - 16 - 0.5f * UIPadding));

    auto [enemyAttack, enemyAttackSprite, enemyAttackLabel, enemyAttackButton]
        = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(characterContainerSize.x - enemyImageSize.x - 4 * UIPadding, buttonHeight), "Lato", "ATTACK", 24, TextAlignment::CENTER, secondaryText)); // todo create button
    enemyAttack.setOrigin(glm::vec2(0.5f, 0.5f));
    enemyAttack.setPosition(glm::vec2(enemyImageSize.x / 2.0f + UIPadding, 2.0f * UIPadding));

    auto [enemyFocus, enemyFocusSprite, enemyFocusLabel, enemyFocusButton]
        = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(characterContainerSize.x - enemyImageSize.x - 4 * UIPadding, buttonHeight), "Lato", "FOCUS", 24, TextAlignment::CENTER, secondaryText)); // todo create button
    enemyFocus.setOrigin(glm::vec2(0.5f, 0.5f));
    enemyFocus.setPosition(glm::vec2(enemyImageSize.x / 2.0f + UIPadding, buttonHeight + 2.5f * UIPadding));

    auto [enemyContainer, enemyContainerBackground] = DeconstructSprite(MakeSprite("assets/textures/square.png", characterContainerSize));
    enemyContainerBackground.setColor(panelLight);
    enemyContainer.setPosition(glm::vec2(windowSize.x - characterContainerSize.x / 2.0f - 4.0f * UIPadding, windowSize.y / 2.0f + characterContainerSize.y / 2.0f));
    enemyContainer.addChild(enemyImageObject);
    enemyContainer.addChild(enemyHealth);
    enemyContainer.addChild(enemyName);
    enemyContainer.addChild(enemyStatus);
    enemyContainer.addChild(enemyAttack);
    enemyContainer.addChild(enemyFocus);

    // todo implement character visualizer which changes sprites, updates health

    auto [setIdleStateObject, sISSprite, sISLabel, sISButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(200, 40), "Lato", "Set Idle", 32,
        TextAlignment::CENTER, sf::Color::Black));
    setIdleStateObject.setPosition(glm::vec2(windowSize.x - 100 - UIPadding,  20 + UIPadding));
    auto [setHurtStateObject, sHSSprite, sHSLabel, sHSButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(200, 40), "Lato", "Set Hurt", 32,
        TextAlignment::CENTER, sf::Color::Black));
    setHurtStateObject.setPosition(glm::vec2(windowSize.x - 100 - UIPadding, 60 + 2.0f * UIPadding));
    auto [setAttackStateObject, sASSprite, sASLabel, sASButton] = DeconstructButton(MakeButton("assets/textures/square.png", glm::vec2(200, 40), "Lato", "Set Attack", 32,
        TextAlignment::CENTER, sf::Color::Black));
    setAttackStateObject.setPosition(glm::vec2(windowSize.x - 100 - UIPadding, 100 + 3.0f*UIPadding));

    auto& playerController = gameContainer.addScriptableBehaviour<PlayerController>(player, playerImageSprite);
    sISButton.setOnClick([&playerController]() {
        playerController.setIdleState();
        });

    sHSButton.setOnClick([&playerController]() {
        playerController.setHurtState();
        });

    sASButton.setOnClick([&playerController]() {
        playerController.setAttackState();
        });

    gameContainer.addChild(playerContainer);
    gameContainer.addChild(enemyContainer);
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

        window.clear(background);
        sceneManager.render(window);
        window.display();
    }
}
