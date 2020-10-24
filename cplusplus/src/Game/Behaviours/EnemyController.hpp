#pragma once
#include "Core/Scene/ScriptableBehaviour.hpp"

class PlayerController;
class Character;

class EnemyController : public ScriptableBehaviour {
public:
    EnemyController(PlayerController& playerController);
    EnemyController(const std::string& identifier, PlayerController& playerController);
protected:
    void onStart() override;
    void onUpdate(sf::Time ts) override;

private:
    void enablePlayer();
    void enemyAct();
    void nextEnemy();
    void loadBaseStats();
private:
    PlayerController& playerController;
    Character* player;
    Character* currentEnemy;
    bool attackPriority; // who starts
    bool currentOrder; // currentOrder^attackPriority: false => player, true => enemy;

    bool waitingForPlayer;
    bool waitingForEnemy;
    float waitForEnemyTimer;

    const inline static int maxTypes = 5;
    const inline static std::string types[maxTypes] = { "player", "adventurer", "female", "soldier", "zombie" };

    struct base_stats {
        int vitality;
        int attack;
        int defense;
        int stamina;
    };
    inline static base_stats baseStats[maxTypes] = { {},{},{},{},{} };
};
