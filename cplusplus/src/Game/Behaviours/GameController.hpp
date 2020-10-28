#pragma once
#include "Core/Scene/ScriptableBehaviour.hpp"

class InfoArea;
class TextRenderer;
class CharacterVisualizer;
class PlayerController;
class Character;

class GameController final : public ScriptableBehaviour {
public:
	GameController(InfoArea& infoArea, PlayerController& playerController, CharacterVisualizer& playerVisualizer,
	                CharacterVisualizer& enemyVisualizer, TextRenderer& playerStatusText, TextRenderer& enemyStatusText);
	GameController(const std::string& identifier, InfoArea& infoArea, PlayerController& playerController,
	                CharacterVisualizer& playerVisualizer, CharacterVisualizer& enemyVisualizer, TextRenderer& playerStatusText, TextRenderer& enemyStatusText);
	void playerAttack();
	void playerHeal();
	void playerFocus();
	void continueGame();
	void saveGame() const;
protected:
	void onStart() override;
	void onUpdate(sf::Time ts) override;
private:
	void reset();
	void enablePlayer();
	void disablePlayer();

	void enemyAct();
	void nextEnemy();
private:
	InfoArea& infoArea;
	PlayerController& playerController;
	CharacterVisualizer& playerVisualizer;
	CharacterVisualizer& enemyVisualizer;
	TextRenderer& playerStatusText;
	TextRenderer& enemyStatusText;
	Character* player;
	Character* currentEnemy;
	int enemiesKilled;
	bool attackPriority; // who starts
	bool currentOrder;   // currentOrder^attackPriority: false => player, true => enemy;
	bool loadedGame;
	
	bool waitingForPlayer;
	bool waitingForEnemy;
	float waitForEnemyTimer;
	float playerStatusTextTimer;
	float enemyStatusTextTimer;
	constexpr inline static float statusTextTime = 1.5f;
};
