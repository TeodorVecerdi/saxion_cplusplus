#include "Core/Core.hpp"
#include "Core/Input.hpp"

#include "Core/Behaviours/SpriteRenderer.hpp"
#include "Core/Behaviours/TextRenderer.hpp"
#include "Core/Behaviours/Button.hpp"
#include "Core/Behaviours/FilledSpriteRenderer.hpp"
#include "Core/Drawing/Theme.hpp"
#include "Core/Scene/Scene.hpp"
#include "Core/Scene/SceneManager.hpp"
#include "Core/Scene/GameObjectTemplates.hpp"

#include "Game/State/Character.hpp"
#include "Game/Behaviours/FPSCounter.hpp"
#include "Game/Behaviours/CreateCharacterController.hpp"
#include "Game/Behaviours/PlayerController.hpp"
#include "Game/Behaviours/CharacterVisualizer.hpp"
#include "Game/Behaviours/ContinueGame.hpp"
#include "Game/Behaviours/GameController.hpp"
#include "Game/Behaviours/GameOverScore.hpp"
#include "Game/Behaviours/InfoArea.hpp"
#include "Game/Behaviours/Ranking.hpp"
#include "Game/State/Difficulty.hpp"
#include "Game/State/Scoreboard.hpp"

#define GET_ARGi(idx,default) idx < argc ? std::stoi(argv[idx]) : default
#define GET_ARGs(idx,default) idx < argc ? argv[idx] : default

int main(int argc, char* argv[]) {
	uint32_t width = GET_ARGi(1, 1600), height = GET_ARGi(2, 900);

	// Clean temp folder
	std::filesystem::path temp("assets/temp");
	std::filesystem::remove_all(temp);
	std::filesystem::create_directory(temp);

	sf::RenderWindow window(sf::VideoMode(width, height), "Hello SFML", sf::Style::Close);
	window.setVerticalSyncEnabled(true);
	sf::Clock deltaClock;
	srand(static_cast<uint32_t>(time(nullptr)));

	Scene createCharacterScene("createCharacter");
	Scene mainMenu("mainMenu");
	Scene difficultyMenu("difficultyMenu");
	Scene gameScene("game");
	Scene gameOverScene("gameOver");
	SceneManager sceneManager;
	sceneManager.registerScene(createCharacterScene);
	sceneManager.registerScene(mainMenu);
	sceneManager.registerScene(difficultyMenu);
	sceneManager.registerScene(gameScene);
	sceneManager.registerScene(gameOverScene);
	sceneManager.stackScene(mainMenu, false);

	mainMenu.addScriptableBehaviour<FPSCounter>(0.5f);
	createCharacterScene.addScriptableBehaviour<FPSCounter>(0.5f);
	difficultyMenu.addScriptableBehaviour<FPSCounter>(0.5f);
	gameScene.addScriptableBehaviour<FPSCounter>(0.5f);

	Scoreboard scoreboard(8);
	std::shared_ptr<Character> player = std::make_shared<Character>();
	player->setIsPlayer(true);

	// Colors
	sf::Color mintCream(246, 255, 255);
	sf::Color imperialRed(233, 54, 64);
	sf::Color pacificBlue(15, 163, 184);
	sf::Color oxfordBlue(0, 0, 25);
	sf::Color independance(73, 71, 91);
	sf::Color raisinBlack(32, 32, 48);

	// Theme
	std::unique_ptr<Theme> mainTheme = std::make_unique<Theme>(Theme{
		.primaryText = mintCream,
		.secondaryText = oxfordBlue,
		.background = oxfordBlue,
		.panelDark = raisinBlack,
		.panelLight = independance,
		.themeColor1 = imperialRed,
		.themeColor2 = pacificBlue,

		.buttonNormal = sf::Color::White,
		.buttonActive = sf::Color(200, 200, 200),
		.buttonHover = sf::Color(220, 220, 220),
		.buttonDisabled = sf::Color(150, 150, 150)
	});
	Theme::SetActiveTheme(mainTheme);

	auto windowSize = window.getSize();
	float UIPadding = 10;

#pragma region Main Menu Scene
	auto [playGame, playGameSprite, playGameLabel, playGameButton] = DeconstructButton(MakeButton(
		"assets/textures/square.png", glm::vec2(200, 40), "Lato", "Play", 24,
		TextAlignment::CENTER, Theme::activeTheme->secondaryText));
	playGame.setPosition(glm::vec2(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 40 - 20 - 1.5f * UIPadding));
	playGameButton.setOnClick([&sceneManager]() {
		sceneManager.popScene();
		sceneManager.stackScene("difficultyMenu", true);
	});

	auto [continueGame, continueGameSprite, continueGameLabel, continueGameButton] = DeconstructButton(MakeButton(
		"assets/textures/square.png", glm::vec2(200, 40), "Lato", "Continue", 24,
		TextAlignment::CENTER, Theme::activeTheme->secondaryText));
	continueGame.setPosition(glm::vec2(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 20 - 0.5f * UIPadding));

	auto [eraseData, eraseDataSprite, eraseDataLabel, eraseDataButton] = DeconstructButton(MakeButton(
		"assets/textures/square.png", glm::vec2(200, 40), "Lato", "Erase Data", 24,
		TextAlignment::CENTER, Theme::activeTheme->secondaryText));
	eraseData.setPosition(glm::vec2(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 20 + 0.5f * UIPadding));

	auto [exitButtonObject, exitSprite, exitLabel, exitButton] = DeconstructButton(MakeButton(
		"assets/textures/square.png", glm::vec2(200, 40), "Lato", "Exit", 24,
		TextAlignment::CENTER, Theme::activeTheme->secondaryText));
	exitButtonObject.setPosition(
		glm::vec2(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 40 + 20 + 1.5f * UIPadding));
	exitButton.setOnClick([&window]() { window.close(); });

	auto [rankingTextContainer, rankingTextContainerSprite] = DeconstructSprite(
		MakeSprite("assets/textures/square.png", glm::vec2(0.5f * windowSize.x - 300, 0.5f * windowSize.y))
	);
	rankingTextContainerSprite.setColor(Theme::activeTheme->panelDark);
	auto [rankingTitle, rankingTitleLabel] = DeconstructLabel(
		MakeLabel("Lato", "RANKING", 32, TextAlignment::CENTER, Theme::activeTheme->primaryText)
	);
	rankingTitle.setPosition(glm::vec2(0, -0.25f * windowSize.y - 16 - UIPadding));

	auto& rankingContainer = MakeEmpty();
	rankingContainer.setPosition(glm::vec2(0.25f * windowSize.x - 50, 0.5f * windowSize.y));
	rankingContainer.addChild(rankingTextContainer);
	rankingContainer.addChild(rankingTitle);
	auto& ranking = rankingContainer.addScriptableBehaviour<Ranking>(24, 16, -0.25f * windowSize.x + 150 + 2.0f*UIPadding, -0.25f * windowSize.y, rankingTextContainerSprite, scoreboard);

	auto& continueGameController = continueGame.addScriptableBehaviour<ContinueGame>(
            continueGameButton, eraseDataButton, ranking, scoreboard);
	continueGameButton.setOnClick([&sceneManager, &continueGameController]() {
        sceneManager.popScene();
        sceneManager.stackScene("game", true);
        continueGameController.continueGame();
    });
	eraseDataButton.setOnClick([&continueGameController]() {
        continueGameController.eraseData();
    });
	
	mainMenu.addChild(playGame);
	mainMenu.addChild(continueGame);
	mainMenu.addChild(eraseData);
	mainMenu.addChild(exitButtonObject);
	mainMenu.addChild(rankingContainer);
#pragma endregion
#pragma region Difficulty Selection
	auto& difficultyContainer = MakeEmpty();
	difficultyContainer.setPosition(glm::vec2(windowSize.x / 2.0f, windowSize.y / 2.0f));
	auto [difficultyTitle, difficultyTitleLabel] = DeconstructLabel(
		MakeLabel("Lato", "Select Difficulty", 32, TextAlignment::CENTER, Theme::activeTheme->primaryText));
	difficultyTitle.setPosition(glm::vec2(0, -80 - 1.5f * UIPadding));
	auto [easy, easySprite, easyLabel, easyButton] = DeconstructButton(MakeButton(
		"assets/textures/square.png", glm::vec2(200, 40), "Lato", capitalize(Difficulty::GetDifficultyName(0)), 24, TextAlignment::CENTER,
		Theme::activeTheme->secondaryText));
	easy.setPosition(glm::vec2(0, -40 - UIPadding));
	easyButton.setOnClick([&sceneManager]() {
		Difficulty::SetActiveDifficulty(0);
		sceneManager.popScene(); // difficulty
		sceneManager.stackScene("createCharacter", true);
	});
	auto [normal, normalSprite, normalLabel, normalButton] = DeconstructButton(MakeButton(
		"assets/textures/square.png", glm::vec2(200, 40), "Lato", capitalize(Difficulty::GetDifficultyName(1)), 24, TextAlignment::CENTER,
		Theme::activeTheme->secondaryText));
	normalButton.setOnClick([&sceneManager]() {
		Difficulty::SetActiveDifficulty(1);
		sceneManager.popScene(); // difficulty
		sceneManager.stackScene("createCharacter", true);
	});
	auto [hard, hardSprite, hardLabel, hardButton] = DeconstructButton(MakeButton(
		"assets/textures/square.png", glm::vec2(200, 40), "Lato", capitalize(Difficulty::GetDifficultyName(2)), 24, TextAlignment::CENTER,
		Theme::activeTheme->secondaryText));
	hard.setPosition(glm::vec2(0, 40 + UIPadding));
	hardButton.setOnClick([&sceneManager]() {
		Difficulty::SetActiveDifficulty(2);
		sceneManager.popScene(); // difficulty
		sceneManager.stackScene("createCharacter", true);
	});
	auto [difficultyMenuBack, difficultyMenuBackSprite, difficultyMenuBackLabel, difficultyMenuBackButton] =
		DeconstructButton(MakeButton(
			"assets/textures/square.png", glm::vec2(175, 32), "Lato", "Back", 24, TextAlignment::CENTER,
			Theme::activeTheme->secondaryText));
	difficultyMenuBack.setPosition(glm::vec2(0, 80 + 1.5f * UIPadding));
	difficultyMenuBackButton.setOnClick([&sceneManager]() {
		sceneManager.popScene();
		sceneManager.stackScene("mainMenu", false);
	});

	difficultyContainer.addChild(difficultyTitle);
	difficultyContainer.addChild(easy);
	difficultyContainer.addChild(normal);
	difficultyContainer.addChild(hard);
	difficultyContainer.addChild(difficultyMenuBack);

	difficultyMenu.addChild(difficultyContainer);
#pragma endregion
#pragma region Create Character Scene
	float headerHeight = 48 + 2 * UIPadding;
	sf::FloatRect col1(0, headerHeight, 3.0f * windowSize.x / 7.0f, windowSize.y - headerHeight);
	sf::FloatRect col2(3.0f * windowSize.x / 7.0f, headerHeight, 3.0f * windowSize.x / 7.0f,
	                   windowSize.y - headerHeight);
	sf::FloatRect col3(6.0f * windowSize.x / 7.0f, 0, 1.0f * windowSize.x / 7.0f, static_cast<float>(windowSize.y));

	auto& createCharacterObject = MakeEmpty();
	createCharacterObject.setOrigin(glm::vec2(0));

	auto [createCharTitleObject, cCTLabel] = DeconstructLabel(
		MakeLabel("Lato", "Create Character", 48, TextAlignment::CENTER, Theme::activeTheme->primaryText));
	createCharTitleObject.setPosition(glm::vec2(col1.left + windowSize.x / 2.0f, headerHeight / 2.0f));
	createCharacterObject.addChild(createCharTitleObject);

	// col1
	auto [characterNameTextObject, cNLabel] = DeconstructLabel(
		MakeLabel("Lato", "", 32, TextAlignment::RIGHT, Theme::activeTheme->primaryText));
	characterNameTextObject.setPosition(glm::vec2(col1.width - 150 - 2 * UIPadding, 2 * UIPadding));
	characterNameTextObject.setOrigin(glm::vec2(0.5f, 0.0f));

	auto [randomizeNameObject, rNSprite, rNLabel, rNButton] = DeconstructButton(MakeButton(
		"assets/textures/square.png", glm::vec2(100, 32), "Lato", "Random", 24,
		TextAlignment::CENTER, Theme::activeTheme->secondaryText));
	randomizeNameObject.setPosition(glm::vec2(col1.width - 75 - UIPadding, 2 * UIPadding));

	auto [characterTypeImageObject, cTSprite] = DeconstructSprite(
		MakeSprite("assets/textures/square.png", glm::vec2(0.857142857f * 0.5f * col1.width, 0.5f * col1.width)));
	characterTypeImageObject.setPosition(
		glm::vec2(col1.width / 2.0f, 2 * UIPadding + cNLabel.getFontSize() + UIPadding));
	characterTypeImageObject.setOrigin(glm::vec2(0.5f, 0.0f));

	auto [characterTypeTextObject, cTTLabel] = DeconstructLabel(
		MakeLabel("Lato", "", 32, TextAlignment::CENTER, Theme::activeTheme->primaryText));
	characterTypeTextObject.setPosition(glm::vec2(col1.width / 2.0f,
	                                              3 * UIPadding + 0.5f * col1.width + cNLabel.getFontSize() + 4 *
	                                              UIPadding));
	characterTypeTextObject.setOrigin(glm::vec2(0.5f, 0.0f));

	auto [decreaseTypeObject, dTSprite, dTLabel, dTButton] = DeconstructButton(MakeButton(
		"assets/textures/square.png", glm::vec2(32), "Lato", "<", 24, TextAlignment::CENTER,
		Theme::activeTheme->secondaryText));
	decreaseTypeObject.setPosition(glm::vec2(16 + 25 + UIPadding,
	                                         3 * UIPadding + 0.5f * col1.width + cNLabel.getFontSize() + 4 *
	                                         UIPadding));

	auto [increaseTypeObject, iTSprite, iTLabel, iTButton] = DeconstructButton(MakeButton(
		"assets/textures/square.png", glm::vec2(32), "Lato", ">", 24, TextAlignment::CENTER,
		Theme::activeTheme->secondaryText));
	increaseTypeObject.setPosition(glm::vec2(col1.width - 16 - 25 - UIPadding,
	                                         3 * UIPadding + 0.5f * col1.width + cNLabel.getFontSize() + 4 *
	                                         UIPadding));

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
	auto [remainingPointsObject, remainingPointsLabel] = DeconstructLabel(
		MakeLabel("Lato", "<NAME>", 32, TextAlignment::LEFT, Theme::activeTheme->primaryText));
	remainingPointsObject.setPosition(glm::vec2(2 * UIPadding, 2 * UIPadding));

	auto [vitalityPointsObject, vitalityPointsLabel] = DeconstructLabel(
		MakeLabel("Lato", "<NAME>", 32, TextAlignment::LEFT, Theme::activeTheme->primaryText));
	auto [vitalityIncreaseObject, vISprite, vILabel, vIButton] = DeconstructButton(MakeButton(
		"assets/textures/square.png", glm::vec2(32), "Lato", "+", 32, TextAlignment::CENTER,
		Theme::activeTheme->secondaryText));
	auto [vitalityDecreaseObject, vDSprite, vDLabel, vDButton] = DeconstructButton(MakeButton(
		"assets/textures/square.png", glm::vec2(32), "Lato", "-", 32, TextAlignment::CENTER,
		Theme::activeTheme->secondaryText));
	vitalityPointsObject.setPosition(glm::vec2(2.0f * UIPadding, lineHeight32 + 3.0f * UIPadding));
	vitalityIncreaseObject.setPosition(glm::vec2(col2.width - 16 - 2.0f * UIPadding, lineHeight32 + 3.0f * UIPadding));
	vitalityDecreaseObject.setPosition(glm::vec2(col2.width - 48 - 3.0f * UIPadding, lineHeight32 + 3.0f * UIPadding));

	auto [damagePointsObject, damagePointsLabel] = DeconstructLabel(
		MakeLabel("Lato", "<NAME>", 32, TextAlignment::LEFT, Theme::activeTheme->primaryText));
	auto [damageIncreaseObject, dmgISprite, dmgILabel, dmgIButton] = DeconstructButton(MakeButton(
		"assets/textures/square.png", glm::vec2(32), "Lato", "+", 32,
		TextAlignment::CENTER, Theme::activeTheme->secondaryText));
	auto [damageDecreaseObject, dmgDSprite, dmgDLabel, dmgDButton] = DeconstructButton(MakeButton(
		"assets/textures/square.png", glm::vec2(32), "Lato", "-", 32,
		TextAlignment::CENTER, Theme::activeTheme->secondaryText));
	damagePointsObject.setPosition(glm::vec2(2.0f * UIPadding, 2.0f * lineHeight32 + 3.5f * UIPadding));
	damageIncreaseObject.setPosition(glm::vec2(col2.width - 16 - 2.0f * UIPadding,
	                                           2.0f * lineHeight32 + 3.5f * UIPadding));
	damageDecreaseObject.setPosition(glm::vec2(col2.width - 48 - 3.0f * UIPadding,
	                                           2.0f * lineHeight32 + 3.5f * UIPadding));

	auto [defensePointsObject, defensePointsLabel] = DeconstructLabel(
		MakeLabel("Lato", "<NAME>", 32, TextAlignment::LEFT, Theme::activeTheme->primaryText));
	auto [defenseIncreaseObject, dfISprite, dfILabel, dfIButton] = DeconstructButton(MakeButton(
		"assets/textures/square.png", glm::vec2(32), "Lato", "+", 32, TextAlignment::CENTER,
		Theme::activeTheme->secondaryText));
	auto [defenseDecreaseObject, dfDSprite, dfDLabel, dfDButton] = DeconstructButton(MakeButton(
		"assets/textures/square.png", glm::vec2(32), "Lato", "-", 32, TextAlignment::CENTER,
		Theme::activeTheme->secondaryText));
	defensePointsObject.setPosition(glm::vec2(2.0f * UIPadding, 3.0f * lineHeight32 + 4.0f * UIPadding));
	defenseIncreaseObject.setPosition(glm::vec2(col2.width - 16 - 2.0f * UIPadding,
	                                            3.0f * lineHeight32 + 4.0f * UIPadding));
	defenseDecreaseObject.setPosition(glm::vec2(col2.width - 48 - 3.0f * UIPadding,
	                                            3.0f * lineHeight32 + 4.0f * UIPadding));

	auto [staminaPointsObject, staminaPointsLabel] = DeconstructLabel(
		MakeLabel("Lato", "<NAME>", 32, TextAlignment::LEFT, Theme::activeTheme->primaryText));
	auto [staminaIncreaseObject, stISprite, stILabel, stIButton] = DeconstructButton(MakeButton(
		"assets/textures/square.png", glm::vec2(32), "Lato", "+", 32, TextAlignment::CENTER,
		Theme::activeTheme->secondaryText));
	auto [staminaDecreaseObject, stDSprite, stDLabel, stDButton] = DeconstructButton(MakeButton(
		"assets/textures/square.png", glm::vec2(32), "Lato", "-", 32, TextAlignment::CENTER,
		Theme::activeTheme->secondaryText));
	staminaPointsObject.setPosition(glm::vec2(2.0f * UIPadding, 4.0f * lineHeight32 + 4.5f * UIPadding));
	staminaIncreaseObject.setPosition(glm::vec2(col2.width - 16 - 2.0f * UIPadding,
	                                            4.0f * lineHeight32 + 4.5f * UIPadding));
	staminaDecreaseObject.setPosition(glm::vec2(col2.width - 48 - 3.0f * UIPadding,
	                                            4.0f * lineHeight32 + 4.5f * UIPadding));

	auto [randomizeStats, randomizeStatsSprite, randomizeStatsLabel, randomizeStatsButton] = DeconstructButton(
		MakeButton("assets/textures/square.png", glm::vec2(col2.width - 4 * UIPadding, 32), "Lato", "Randomize stats",
		           24, TextAlignment::CENTER, Theme::activeTheme->secondaryText)
	);
	randomizeStats.setPosition(glm::vec2(col2.width / 2.0f, 5.0f * lineHeight32 + 5.0f * UIPadding));

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
	col2Object.addChild(randomizeStats);

	createCharacterObject.addChild(col2Object);

	// col3
	auto [finishCreatingCharacterObject, fCCSprite, fCCLabel, fCCButton] = DeconstructButton(
		MakeButton("assets/textures/square.png", glm::vec2(col3.width - 2 * UIPadding, 40), "Lato", "Create", 24,
		           TextAlignment::CENTER, Theme::activeTheme->secondaryText));
	finishCreatingCharacterObject.setPosition(glm::vec2(col3.width / 2.0f, col3.height - 30 - 40 - 10));

	auto [backFromCharacterCreationObject, bCCSprite, bCCLabel, bCCButton] = DeconstructButton(
		MakeButton("assets/textures/square.png", glm::vec2(col3.width - 2 * UIPadding, 40), "Lato", "Back", 24,
		           TextAlignment::CENTER, Theme::activeTheme->secondaryText));
	backFromCharacterCreationObject.setPosition(glm::vec2(col3.width / 2.0f, col3.height - 30));

	auto& col3Object = MakeEmpty();
	col3Object.setOrigin(glm::vec2(0));
	col3Object.addChild(finishCreatingCharacterObject);
	col3Object.addChild(backFromCharacterCreationObject);
	col3Object.setPosition(glm::vec2(col3.left, col3.top));
	createCharacterObject.addChild(col3Object);

	auto& createCharacterController = createCharacterObject.addScriptableBehaviour<CreateCharacterController>(
		&cTSprite, &cNLabel, &cTTLabel, &vitalityPointsLabel, &damagePointsLabel, &defensePointsLabel,
		&staminaPointsLabel, &remainingPointsLabel);

	fCCButton.setOnClick([&sceneManager, &createCharacterController, &player]() {
		createCharacterController.exportCharacter();
		player->loadFromFile(createCharacterController.getExportPath());
		sceneManager.popScene(); // pop character creation
		sceneManager.stackScene("game", false);
	});
	bCCButton.setOnClick([&sceneManager]() {
		sceneManager.popScene();
		sceneManager.stackScene("mainMenu", false);
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
		createCharacterController.changeAttack(-1);
	});
	dmgIButton.setOnClick([&createCharacterController]() {
		createCharacterController.changeAttack(1);
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
	randomizeStatsButton.setOnClick([&createCharacterController]() {
		createCharacterController.randomizeStats();
	});
	createCharacterScene.addChild(createCharacterObject);

#pragma endregion
#pragma region Game Scene
	auto& gameContainer = MakeEmpty();

	auto characterContainerSize = glm::vec2(0.5f * windowSize.x - 12.f * UIPadding,
	                                        0.5f * windowSize.y - 12.f * UIPadding);

	auto playerImageSize = glm::vec2(1.8f / 5.0f * characterContainerSize.x,
	                                 1.8f / 5.0f * characterContainerSize.x * (54.0f / 48.0f));
	auto [playerImageObject, playerImageSprite] = DeconstructSprite(
		MakeSprite("assets/textures/square.png", playerImageSize));
	playerImageObject.setPosition(glm::vec2(-characterContainerSize.x / 2.0f + playerImageSize.x / 2.0f + UIPadding,
	                                        0));

	auto& playerHealth = MakeEmpty();
	playerHealth.setPosition(glm::vec2(-(characterContainerSize.x / 2.0f - playerImageSize.x - 3 * UIPadding),
	                                   -characterContainerSize.y / 2 + 20.0f + UIPadding));
	playerHealth.setOrigin(glm::vec2(0.0f, 0.5f));
	auto& playerHealthSpriteBackground = playerHealth.addScriptableBehaviour<SpriteRenderer>(
		"assets/textures/square.png", false);
	playerHealthSpriteBackground.setColor(Theme::activeTheme->background);
	playerHealthSpriteBackground.setSize(glm::vec2(characterContainerSize.x - playerImageSize.x - 4 * UIPadding, 40));
	auto& playerHealthSprite = playerHealth.addScriptableBehaviour<FilledSpriteRenderer>(
		"assets/textures/square.png", false);
	playerHealthSprite.setSize(glm::vec2(characterContainerSize.x - playerImageSize.x - 4 * UIPadding, 40));
	playerHealthSprite.setColor(Theme::activeTheme->themeColor1);
	playerHealthSprite.setFillAmount(1);
	auto [playerHealthLabel1, playerHealthLabel1Text] = DeconstructLabel(
		MakeLabel("Lato", "Health:", 24, TextAlignment::LEFT, Theme::activeTheme->primaryText));
	playerHealthLabel1.setPosition(glm::vec2(UIPadding, 0));
	auto [playerHealthLabel2, playerHealthLabel2Text] = DeconstructLabel(
		MakeLabel("Lato", "##/##", 24, TextAlignment::RIGHT, Theme::activeTheme->primaryText));
	playerHealthLabel2.setPosition(
		glm::vec2(characterContainerSize.x / 2.0f + playerImageSize.x / 2.0f - (4 + 2 + 1.5f) * UIPadding, 0));
	playerHealth.addChild(playerHealthLabel1);
	playerHealth.addChild(playerHealthLabel2);

	auto [playerName, playerNameLabel] = DeconstructLabel(
		MakeLabel("Lato", "<CharacterName>", 32, TextAlignment::LEFT, Theme::activeTheme->primaryText));
	playerName.setPosition(glm::vec2(-characterContainerSize.x / 2.0f,
	                                 -characterContainerSize.y / 2.0f - 16 - 0.5f * UIPadding));

	auto [playerStatus, playerStatusLabel] = DeconstructLabel(
		MakeLabel("Lato", "...<Status>", 32, TextAlignment::RIGHT, Theme::activeTheme->primaryText));
	playerStatus.setPosition(glm::vec2(characterContainerSize.x / 2.0f,
	                                   -characterContainerSize.y / 2.0f - 16 - 0.5f * UIPadding));

	const float buttonSpaceRemaining = characterContainerSize.y - 80 - 4.5f * UIPadding;
	const float buttonHeight = buttonSpaceRemaining / 4.0f;

	auto [playerAttack, playerAttackSprite, playerAttackLabel, playerAttackButton]
		= DeconstructButton(MakeButton("assets/textures/square.png",
		                               glm::vec2(characterContainerSize.x - playerImageSize.x - 4 * UIPadding,
		                                         buttonHeight), "Lato", "ATTACK", 24, TextAlignment::CENTER,
		                               Theme::activeTheme->secondaryText));
	playerAttack.setPosition(glm::vec2(playerImageSize.x / 2.0f + UIPadding,
	                                   characterContainerSize.y / 2.0f - 2.5f * buttonHeight - 2.0f * UIPadding));

	auto [playerFocus, playerFocusSprite, playerFocusLabel, playerFocusButton]
		= DeconstructButton(MakeButton("assets/textures/square.png",
		                               glm::vec2(characterContainerSize.x - playerImageSize.x - 4 * UIPadding,
		                                         buttonHeight), "Lato", "FOCUS", 24, TextAlignment::CENTER,
		                               Theme::activeTheme->secondaryText));
	playerFocus.setPosition(glm::vec2(playerImageSize.x / 2.0f + UIPadding,
	                                  characterContainerSize.y / 2.0f - 1.5f * buttonHeight - 1.5f * UIPadding));

	auto [playerHeal, playerHealSprite, playerHealLabel, playerHealButton]
		= DeconstructButton(MakeButton("assets/textures/square.png",
		                               glm::vec2(characterContainerSize.x - playerImageSize.x - 4 * UIPadding,
		                                         buttonHeight), "Lato", "HEAL", 24, TextAlignment::CENTER,
		                               Theme::activeTheme->secondaryText));
	playerHeal.setPosition(glm::vec2(playerImageSize.x / 2.0f + UIPadding,
	                                 characterContainerSize.y / 2.0f - 0.5f * buttonHeight - 1.0f * UIPadding));

	auto [playerVitAttrSpriteContainer, playerVitAttrSprite] = DeconstructSprite(
		MakeSprite("assets/textures/attributes/vitality.png", glm::vec2(32 + 2.0f * UIPadding)));
	playerVitAttrSpriteContainer.setPosition(glm::vec2(0, -12));
	auto [playerVitAttrLabelContainer, playerVitAttrLabel] = DeconstructLabel(
		MakeLabel("Lato", "#", 24, TextAlignment::CENTER, Theme::activeTheme->primaryText));
	playerVitAttrLabelContainer.setPosition(glm::vec2(0, 16 + 1.0f * UIPadding));
	auto [playerVitAttrContainer, playerVitAttrBackground] = DeconstructSprite(
		MakeSprite("assets/textures/square.png", glm::vec2(32 + 2.0f * UIPadding, 56 + 3.0f * UIPadding)));
	playerVitAttrBackground.setColor(Theme::activeTheme->themeColor2);
	playerVitAttrContainer.setPosition(glm::vec2(
		-0.5f * characterContainerSize.x + playerImageSize.x + 4.0f * UIPadding + 16,
		-0.5f * characterContainerSize.y + 68.0f + 3.5f * UIPadding));
	playerVitAttrContainer.addChild(playerVitAttrSpriteContainer);
	playerVitAttrContainer.addChild(playerVitAttrLabelContainer);

	auto [playerAtkAttrSpriteContainer, playerAtkAttrSprite] = DeconstructSprite(
        MakeSprite("assets/textures/attributes/attack.png", glm::vec2(32 + 2.0f * UIPadding)));
	playerAtkAttrSpriteContainer.setPosition(glm::vec2(0, -12));
	auto [playerAtkAttrLabelContainer, playerAtkAttrLabel] = DeconstructLabel(
        MakeLabel("Lato", "#", 24, TextAlignment::CENTER, Theme::activeTheme->primaryText));
	playerAtkAttrLabelContainer.setPosition(glm::vec2(0, 16 + 1.0f * UIPadding));
	auto [playerAtkAttrContainer, playerAtkAttrBackground] = DeconstructSprite(
        MakeSprite("assets/textures/square.png", glm::vec2(32 + 2.0f * UIPadding, 56 + 3.0f * UIPadding)));
	playerAtkAttrBackground.setColor(Theme::activeTheme->themeColor2);
	playerAtkAttrContainer.setPosition(glm::vec2(
        -0.5f * characterContainerSize.x + playerImageSize.x + 6.5f * UIPadding + 48,
        -0.5f * characterContainerSize.y + 68.0f + 3.5f * UIPadding));
	playerAtkAttrContainer.addChild(playerAtkAttrSpriteContainer);
	playerAtkAttrContainer.addChild(playerAtkAttrLabelContainer);

	auto [playerDefAttrSpriteContainer, playerDefAttrSprite] = DeconstructSprite(
        MakeSprite("assets/textures/attributes/defense.png", glm::vec2(32 + 2.0f * UIPadding)));
	playerDefAttrSpriteContainer.setPosition(glm::vec2(0, -12));
	auto [playerDefAttrLabelContainer, playerDefAttrLabel] = DeconstructLabel(
        MakeLabel("Lato", "#", 24, TextAlignment::CENTER, Theme::activeTheme->primaryText));
	playerDefAttrLabelContainer.setPosition(glm::vec2(0, 16 + 1.0f * UIPadding));
	auto [playerDefAttrContainer, playerDefAttrBackground] = DeconstructSprite(
        MakeSprite("assets/textures/square.png", glm::vec2(32 + 2.0f * UIPadding, 56 + 3.0f * UIPadding)));
	playerDefAttrBackground.setColor(Theme::activeTheme->themeColor2);
	playerDefAttrContainer.setPosition(glm::vec2(
        -0.5f * characterContainerSize.x + playerImageSize.x + 9.0f * UIPadding + 80,
        -0.5f * characterContainerSize.y + 68.0f + 3.5f * UIPadding));
	playerDefAttrContainer.addChild(playerDefAttrSpriteContainer);
	playerDefAttrContainer.addChild(playerDefAttrLabelContainer);

	auto [playerStmAttrSpriteContainer, playerStmAttrSprite] = DeconstructSprite(
        MakeSprite("assets/textures/attributes/stamina.png", glm::vec2(32 + 2.0f * UIPadding)));
	playerStmAttrSpriteContainer.setPosition(glm::vec2(0, -12));
	auto [playerStmAttrLabelContainer, playerStmAttrLabel] = DeconstructLabel(
        MakeLabel("Lato", "#", 24, TextAlignment::CENTER, Theme::activeTheme->primaryText));
	playerStmAttrLabelContainer.setPosition(glm::vec2(0, 16 + 1.0f * UIPadding));
	auto [playerStmAttrContainer, playerStmAttrBackground] = DeconstructSprite(
        MakeSprite("assets/textures/square.png", glm::vec2(32 + 2.0f * UIPadding, 56 + 3.0f * UIPadding)));
	playerStmAttrBackground.setColor(Theme::activeTheme->themeColor2);
	playerStmAttrContainer.setPosition(glm::vec2(
        -0.5f * characterContainerSize.x + playerImageSize.x + 11.5f * UIPadding + 112,
        -0.5f * characterContainerSize.y + 68.0f + 3.5f * UIPadding));
	playerStmAttrContainer.addChild(playerStmAttrSpriteContainer);
	playerStmAttrContainer.addChild(playerStmAttrLabelContainer);

	auto [playerContainer, playerContainerBackground] = DeconstructSprite(
		MakeSprite("assets/textures/square.png", characterContainerSize));
	playerContainerBackground.setColor(Theme::activeTheme->panelLight);
	playerContainer.setPosition(glm::vec2(characterContainerSize.x / 2.0f + 4.0f * UIPadding,
	                                      windowSize.y / 2.0f - characterContainerSize.y / 2.0f));

	auto [playerFocusIcon, playerFocusIconSprite] = DeconstructSprite(
		MakeSprite("assets/textures/status/focus.png", glm::vec2(64))
	);
	playerFocusIcon.setPosition(glm::vec2(-0.5f * characterContainerSize.x + 32 + UIPadding,
	                                      0.5f * characterContainerSize.y - 32 - UIPadding));
	playerFocusIconSprite.setSmooth(true);

	playerContainer.addChild(playerImageObject);
	playerContainer.addChild(playerHealth);
	playerContainer.addChild(playerName);
	playerContainer.addChild(playerStatus);
	playerContainer.addChild(playerAttack);
	playerContainer.addChild(playerFocus);
	playerContainer.addChild(playerHeal);
	playerContainer.addChild(playerFocusIcon);
	playerContainer.addChild(playerVitAttrContainer);
	playerContainer.addChild(playerAtkAttrContainer);
	playerContainer.addChild(playerDefAttrContainer);
	playerContainer.addChild(playerStmAttrContainer);

	auto enemyImageSize = glm::vec2(1.8f / 5.0f * characterContainerSize.x,
	                                1.8f / 5.0f * characterContainerSize.x * (54.0f / 48.0f));
	auto [enemyImageObject, enemyImageSprite] = DeconstructSprite(
		MakeSprite("assets/textures/square.png", enemyImageSize));
	enemyImageObject.setPosition(glm::vec2(-characterContainerSize.x / 2.0f + enemyImageSize.x / 2.0f + UIPadding, 0));

	auto& enemyHealth = MakeEmpty();
	enemyHealth.setPosition(glm::vec2(-(characterContainerSize.x / 2.0f - enemyImageSize.x - 3 * UIPadding),
	                                  -characterContainerSize.y / 2 + 20.0f + UIPadding));
	enemyHealth.setOrigin(glm::vec2(0.0f, 0.5f));
	auto& enemyHealthSpriteBackground = enemyHealth.addScriptableBehaviour<SpriteRenderer>(
		"assets/textures/square.png", false);
	enemyHealthSpriteBackground.setSize(glm::vec2(characterContainerSize.x - enemyImageSize.x - 4 * UIPadding, 40));
	enemyHealthSpriteBackground.setColor(Theme::activeTheme->background);
	auto& enemyHealthSprite = enemyHealth.addScriptableBehaviour<FilledSpriteRenderer>(
		"assets/textures/square.png", false);
	enemyHealthSprite.setSize(glm::vec2(characterContainerSize.x - enemyImageSize.x - 4 * UIPadding, 40));
	enemyHealthSprite.setColor(Theme::activeTheme->themeColor1);
	enemyHealthSprite.setFillAmount(1);
	auto [enemyHealthLabel1, enemyHealthLabel1Text] = DeconstructLabel(
		MakeLabel("Lato", "Health:", 24, TextAlignment::LEFT, Theme::activeTheme->primaryText));
	enemyHealthLabel1.setPosition(glm::vec2(UIPadding, 0));
	auto [enemyHealthLabel2, enemyHealthLabel2Text] = DeconstructLabel(
		MakeLabel("Lato", "##/##", 24, TextAlignment::RIGHT, Theme::activeTheme->primaryText));
	enemyHealthLabel2.setPosition(
		glm::vec2(characterContainerSize.x / 2.0f + enemyImageSize.x / 2.0f - (4 + 2 + 1.5f) * UIPadding, 0));
	enemyHealth.addChild(enemyHealthLabel1);
	enemyHealth.addChild(enemyHealthLabel2);

	auto [enemyStatus, enemyStatusLabel] = DeconstructLabel(
		MakeLabel("Lato", "<Status>...", 32, TextAlignment::LEFT, Theme::activeTheme->primaryText));
	enemyStatus.setPosition(glm::vec2(-characterContainerSize.x / 2.0f,
	                                  -characterContainerSize.y / 2.0f - 16 - 0.5f * UIPadding));

	auto [enemyName, enemyNameLabel] = DeconstructLabel(
		MakeLabel("Lato", "<EnemyName>", 32, TextAlignment::RIGHT, Theme::activeTheme->primaryText));
	enemyName.setPosition(glm::vec2(characterContainerSize.x / 2.0f,
	                                -characterContainerSize.y / 2.0f - 16 - 0.5f * UIPadding));

	auto [enemyVitAttrSpriteContainer, enemyVitAttrSprite] = DeconstructSprite(
		MakeSprite("assets/textures/attributes/vitality.png", glm::vec2(32 + 2.0f * UIPadding)));
	enemyVitAttrSpriteContainer.setPosition(glm::vec2(0, -12));
	auto [enemyVitAttrLabelContainer, enemyVitAttrLabel] = DeconstructLabel(
		MakeLabel("Lato", "#", 24, TextAlignment::CENTER, Theme::activeTheme->primaryText));
	enemyVitAttrLabelContainer.setPosition(glm::vec2(0, 16 + 1.0f * UIPadding));
	auto [enemyVitAttrContainer, enemyVitAttrBackground] = DeconstructSprite(
		MakeSprite("assets/textures/square.png", glm::vec2(32 + 2.0f * UIPadding, 56 + 3.0f * UIPadding)));
	enemyVitAttrBackground.setColor(Theme::activeTheme->themeColor2);
	enemyVitAttrContainer.setPosition(glm::vec2(
		-0.5f * characterContainerSize.x + enemyImageSize.x + 4.0f * UIPadding + 16,
		-0.5f * characterContainerSize.y + 68.0f + 3.5f * UIPadding));
	enemyVitAttrContainer.addChild(enemyVitAttrSpriteContainer);
	enemyVitAttrContainer.addChild(enemyVitAttrLabelContainer);

	auto [enemyAtkAttrSpriteContainer, enemyAtkAttrSprite] = DeconstructSprite(
        MakeSprite("assets/textures/attributes/attack.png", glm::vec2(32 + 2.0f * UIPadding)));
	enemyAtkAttrSpriteContainer.setPosition(glm::vec2(0, -12));
	auto [enemyAtkAttrLabelContainer, enemyAtkAttrLabel] = DeconstructLabel(
        MakeLabel("Lato", "#", 24, TextAlignment::CENTER, Theme::activeTheme->primaryText));
	enemyAtkAttrLabelContainer.setPosition(glm::vec2(0, 16 + 1.0f * UIPadding));
	auto [enemyAtkAttrContainer, enemyAtkAttrBackground] = DeconstructSprite(
        MakeSprite("assets/textures/square.png", glm::vec2(32 + 2.0f * UIPadding, 56 + 3.0f * UIPadding)));
	enemyAtkAttrBackground.setColor(Theme::activeTheme->themeColor2);
	enemyAtkAttrContainer.setPosition(glm::vec2(
        -0.5f * characterContainerSize.x + enemyImageSize.x + 6.5f * UIPadding + 48,
        -0.5f * characterContainerSize.y + 68.0f + 3.5f * UIPadding));
	enemyAtkAttrContainer.addChild(enemyAtkAttrSpriteContainer);
	enemyAtkAttrContainer.addChild(enemyAtkAttrLabelContainer);

	auto [enemyDefAttrSpriteContainer, enemyDefAttrSprite] = DeconstructSprite(
        MakeSprite("assets/textures/attributes/defense.png", glm::vec2(32 + 2.0f * UIPadding)));
	enemyDefAttrSpriteContainer.setPosition(glm::vec2(0, -12));
	auto [enemyDefAttrLabelContainer, enemyDefAttrLabel] = DeconstructLabel(
        MakeLabel("Lato", "#", 24, TextAlignment::CENTER, Theme::activeTheme->primaryText));
	enemyDefAttrLabelContainer.setPosition(glm::vec2(0, 16 + 1.0f * UIPadding));
	auto [enemyDefAttrContainer, enemyDefAttrBackground] = DeconstructSprite(
        MakeSprite("assets/textures/square.png", glm::vec2(32 + 2.0f * UIPadding, 56 + 3.0f * UIPadding)));
	enemyDefAttrBackground.setColor(Theme::activeTheme->themeColor2);
	enemyDefAttrContainer.setPosition(glm::vec2(
        -0.5f * characterContainerSize.x + enemyImageSize.x + 9.0f * UIPadding + 80,
        -0.5f * characterContainerSize.y + 68.0f + 3.5f * UIPadding));
	enemyDefAttrContainer.addChild(enemyDefAttrSpriteContainer);
	enemyDefAttrContainer.addChild(enemyDefAttrLabelContainer);

	auto [enemyStmAttrSpriteContainer, enemyStmAttrSprite] = DeconstructSprite(
        MakeSprite("assets/textures/attributes/stamina.png", glm::vec2(32 + 2.0f * UIPadding)));
	enemyStmAttrSpriteContainer.setPosition(glm::vec2(0, -12));
	auto [enemyStmAttrLabelContainer, enemyStmAttrLabel] = DeconstructLabel(
        MakeLabel("Lato", "#", 24, TextAlignment::CENTER, Theme::activeTheme->primaryText));
	enemyStmAttrLabelContainer.setPosition(glm::vec2(0, 16 + 1.0f * UIPadding));
	auto [enemyStmAttrContainer, enemyStmAttrBackground] = DeconstructSprite(
        MakeSprite("assets/textures/square.png", glm::vec2(32 + 2.0f * UIPadding, 56 + 3.0f * UIPadding)));
	enemyStmAttrBackground.setColor(Theme::activeTheme->themeColor2);
	enemyStmAttrContainer.setPosition(glm::vec2(
        -0.5f * characterContainerSize.x + enemyImageSize.x + 11.5f * UIPadding + 112,
        -0.5f * characterContainerSize.y + 68.0f + 3.5f * UIPadding));
	enemyStmAttrContainer.addChild(enemyStmAttrSpriteContainer);
	enemyStmAttrContainer.addChild(enemyStmAttrLabelContainer);

	auto [enemyContainer, enemyContainerBackground] = DeconstructSprite(
		MakeSprite("assets/textures/square.png", characterContainerSize));
	enemyContainerBackground.setColor(Theme::activeTheme->panelLight);
	enemyContainer.setPosition(glm::vec2(windowSize.x - characterContainerSize.x / 2.0f - 4.0f * UIPadding,
	                                     windowSize.y / 2.0f + characterContainerSize.y / 2.0f));
	enemyContainer.addChild(enemyImageObject);
	enemyContainer.addChild(enemyHealth);
	enemyContainer.addChild(enemyName);
	enemyContainer.addChild(enemyStatus);
	enemyContainer.addChild(enemyVitAttrContainer);
	enemyContainer.addChild(enemyAtkAttrContainer);
	enemyContainer.addChild(enemyDefAttrContainer);
	enemyContainer.addChild(enemyStmAttrContainer);

	auto [infoArea, infoAreaSprite] = DeconstructSprite(MakeSprite("assets/textures/square.png",
	                                                               glm::vec2(characterContainerSize.x,
	                                                                         characterContainerSize.y)));
	infoArea.setPosition(glm::vec2(characterContainerSize.x / 2.0f + 4.0f * UIPadding,
	                               windowSize.y / 2.0f + characterContainerSize.y / 2.0f + 9.0f * UIPadding));
	infoAreaSprite.setColor(Theme::activeTheme->panelLight);

	auto& infoAreaController = infoArea.addScriptableBehaviour<InfoArea>(
		18, 8, UIPadding, -characterContainerSize.x / 2.0f + 2.0f * UIPadding,
		-characterContainerSize.y / 2.0f, infoAreaSprite);
	auto [infoAreaTitle, infoAreaTitleLabel] = DeconstructLabel(
		MakeLabel("Lato", "Battle Log", 32, TextAlignment::LEFT, Theme::activeTheme->primaryText)
	);
	infoAreaTitle.setPosition(glm::vec2(-0.5f * characterContainerSize.x,
	                                    -0.5f * characterContainerSize.y - 16.0f - 0.5f * UIPadding));
	infoArea.addChild(infoAreaTitle);

	auto [fleeBattle, fleeBattleSprite, fleeBattleLabel, fleeBattleButton] = DeconstructButton(
		MakeButton("assets/textures/square.png", glm::vec2(200, 40), "Lato", "Flee Battle", 24, TextAlignment::CENTER,
		           Theme::activeTheme->secondaryText)
	);
	fleeBattle.setPosition(glm::vec2(windowSize.x - 100.0f - 4.0f * UIPadding,
	                                 windowSize.y - 20.0f - 3.0f * UIPadding));

	auto& playerController = gameContainer.addScriptableBehaviour<PlayerController>(
		player, playerAttackButton, playerHealButton, playerFocusButton);
	auto& playerVisualizer = gameContainer.addScriptableBehaviour<CharacterVisualizer>(
		player, &playerImageSprite, &playerHealthSprite, &playerHealthLabel2Text, &playerNameLabel, &playerVitAttrLabel, &playerAtkAttrLabel, &playerDefAttrLabel, &playerStmAttrLabel);
	auto& enemyVisualizer = gameContainer.addScriptableBehaviour<CharacterVisualizer>(
		nullptr, &enemyImageSprite, &enemyHealthSprite, &enemyHealthLabel2Text, &enemyNameLabel, &enemyVitAttrLabel, &enemyAtkAttrLabel, &enemyDefAttrLabel, &enemyStmAttrLabel);
	auto& gameController = gameContainer.addScriptableBehaviour<GameController>(
		infoAreaController, playerController, playerVisualizer, enemyVisualizer, playerStatusLabel, enemyStatusLabel,
		playerFocusIconSprite);
	gameController.setOnPlayerDeath(
		[&scoreboard, &sceneManager](const int difficulty, const int score, const std::string playerName) {
			scoreboard.addScore({difficulty, score, playerName});
			sceneManager.popScene();
			sceneManager.stackScene("gameOver", true);
		}
	);

	continueGameController.setGameController(&gameController);
	playerAttackButton.setOnClick([&gameController]() { gameController.playerAttack(); });
	playerHealButton.setOnClick([&gameController]() { gameController.playerHeal(); });
	playerFocusButton.setOnClick([&gameController]() { gameController.playerFocus(); });
	fleeBattleButton.setOnClick([&gameController, &sceneManager]() {
		gameController.saveGame();
		sceneManager.popScene();
		sceneManager.stackScene("mainMenu", true);
	});

	gameContainer.addChild(playerContainer);
	gameContainer.addChild(enemyContainer);
	gameContainer.addChild(infoArea);
	gameContainer.addChild(fleeBattle);
	gameScene.addChild(gameContainer);
#pragma endregion

#pragma region Game Over Scene
	auto [backToMenu, backToMenuSprite, backToMenuLabel, backToMenuButton] = DeconstructButton(
		MakeButton("assets/textures/square.png", glm::vec2(200, 40), "Lato",
							"Back", 32, TextAlignment::CENTER, Theme::activeTheme->secondaryText)
	);
	backToMenu.setPosition(glm::vec2(0, 0.5f * windowSize.y - 20 - UIPadding));
	backToMenuButton.setOnClick([&sceneManager]() {
		sceneManager.popScene();
		sceneManager.stackScene("mainMenu", true);
	});

	auto& gameOverText = MakeLabel("Lato", "YOU DIED!", 64, TextAlignment::CENTER, Theme::activeTheme->primaryText);
	gameOverText.setPosition(glm::vec2(0, -48 - UIPadding));
	auto [scoreText, scoreTextLabel] = DeconstructLabel(MakeLabel("Lato", "SCORE: ###", 32, TextAlignment::CENTER, Theme::activeTheme->primaryText));
	scoreText.addScriptableBehaviour<GameOverScore>(scoreTextLabel, scoreboard);

	auto& gameOverContainer = MakeEmpty();
	gameOverContainer.setPosition(glm::vec2(0.5f*windowSize.x, 0.5f * windowSize.y));
	gameOverContainer.addChild(backToMenu);
	gameOverContainer.addChild(gameOverText);
	gameOverContainer.addChild(scoreText);
	
	gameOverScene.addChild(gameOverContainer);
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

			if (evt.type == sf::Event::Closed || (evt.type == sf::Event::KeyPressed && evt.key.code ==
				sf::Keyboard::Key::Escape)) {
				window.close();
			}
		}

		sceneManager.update(timeStep);
		Input::resetHitCounters();

		window.clear(Theme::activeTheme->background);
		sceneManager.render(window);
		window.display();
	}
}
