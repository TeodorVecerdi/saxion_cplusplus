#include "EnemyController.hpp"


#include "PlayerController.hpp"
#include "Core/IO.hpp"
#include "Game/Character.hpp"

EnemyController::EnemyController(PlayerController& playerController) : EnemyController(default_identifier(this), playerController) {}

EnemyController::EnemyController(const std::string& identifier, PlayerController& playerController) : ScriptableBehaviour(identifier), playerController(playerController) {
    loadBaseStats();
}

void EnemyController::onStart() {
    player = playerController.getPlayer();
}

void EnemyController::onUpdate(sf::Time ts) {
    if (currentEnemy == nullptr) nextEnemy();

    bool playerAttacks = attackPriority ^ currentOrder;
    if (playerAttacks && !waitingForPlayer) {
        enablePlayer();
        waitingForPlayer = true;
    } else if (!playerAttacks && !waitingForEnemy) {
        waitingForEnemy = true;
        waitForEnemyTimer = (float)(rand() % 30 + 1) / 10.0f; // wait between 1 and 3 seconds for enemy action
    }

    if (waitingForEnemy) {
        waitForEnemyTimer -= ts.asSeconds();
        if (waitForEnemyTimer <= 0.0f) {
            waitingForEnemy = false;
            enemyAct();
            currentOrder = !currentOrder;
        }
    }

    if (currentEnemy->getHealth() <= 0) {
        delete currentEnemy;
        currentEnemy = nullptr;
    }
    if (player->getHealth() <= 0) {
        // TODO: Implement player death.
    }
}

void EnemyController::enablePlayer() {
    playerController.enableControls();
}

void EnemyController::enemyAct() {
    const bool shouldAttack = rand() % 4 != 0; // 25% chance to heal, 75% chance to attack
    if (shouldAttack) {
        const int attackPower = currentEnemy->getAttack() + rand() % 5 - 2; // +/- 2 attack
        player->takeDamage(attackPower);
    } else {
        currentEnemy->heal();
    }
}

void EnemyController::nextEnemy() {
    const int enemyTypeIndex = rand() % maxTypes;
    const auto characterType = types[enemyTypeIndex];
    const auto name = capitalize(characterType);

    const int remainingPoints = 10; // TODO: Take into account player level
    const int split1 = rand() % remainingPoints;
    const int splitLeft = rand() % split1;
    const int splitRight = rand() % (remainingPoints - split1);

    const int vitality = splitLeft;
    const int attack = split1 - splitLeft;
    const int defense = splitRight - split1;
    const int stamina = remainingPoints - splitRight;

    currentEnemy = new Character(name, characterType, vitality + baseStats[enemyTypeIndex].vitality, attack + baseStats[enemyTypeIndex].attack,
                                 defense + baseStats[enemyTypeIndex].defense, stamina + baseStats[enemyTypeIndex].stamina);

    currentOrder = false;
    attackPriority = rand() % 2 == 0;
}

void EnemyController::loadBaseStats() {
    for (int i = 0; i < maxTypes; i++) {
        if (IO::BeginRead(string_format("assets/data/base_character_stats/%s.txt", types[i].c_str()))) {
            IO::ReadInt(baseStats[i].vitality);
            IO::ReadInt(baseStats[i].attack);
            IO::ReadInt(baseStats[i].defense);
            IO::ReadInt(baseStats[i].stamina);
            IO::EndRead();
        }
    }
}
