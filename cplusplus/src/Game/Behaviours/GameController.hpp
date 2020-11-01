#pragma once
#include <functional>

#include "Core/Scene/ScriptableBehaviour.hpp"

class SpriteRenderer;
class InfoArea;
class TextRenderer;
class CharacterVisualizer;
class PlayerController;
class Character;

class GameController final : public ScriptableBehaviour {
public:
	GameController(InfoArea& infoArea, PlayerController& playerController, CharacterVisualizer& playerVisualizer,
	                CharacterVisualizer& enemyVisualizer, TextRenderer& playerStatusText, TextRenderer& enemyStatusText, SpriteRenderer& focusRenderer);
	GameController(const std::string& identifier, InfoArea& infoArea, PlayerController& playerController,
	                CharacterVisualizer& playerVisualizer, CharacterVisualizer& enemyVisualizer, TextRenderer& playerStatusText, TextRenderer& enemyStatusText, SpriteRenderer& focusRenderer);
	void playerAttack();
	void playerHeal();
	void playerFocus();
	void beginFocus();
	void endFocus();
	void continueGame();
	void saveGame() const;
	void setOnPlayerDeath(std::function<void(int,int,std::string)> onPlayerDeath);
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
	SpriteRenderer& focusRenderer;
	std::shared_ptr<Character> player;
	std::shared_ptr<Character> currentEnemy;
	int enemiesKilled;
	bool attackPriority; // who starts
	bool currentOrder;   // currentOrder^attackPriority: false => player, true => enemy;
	bool loadedGame;
	bool playerFocused;
	
	bool waitingForPlayer;
	bool waitingForEnemy;
	float waitForEnemyTimer;
	float playerStatusTextTimer;
	float enemyStatusTextTimer;
	std::function<void(int,int,std::string)> onPlayerDeath;
	
	constexpr inline static float statusTextTime = 1.5f;
};
