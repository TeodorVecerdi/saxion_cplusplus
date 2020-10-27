#pragma once
#include <string>

#include "DifficultySettings.hpp"
#include "Core/IO.hpp"
#include "Utils/StringUtils.hpp"

class Difficulty {
public:
	static void SetActiveDifficulty(const int activeDifficulty) {
		Difficulty::activeDifficulty = activeDifficulty;
	}

	static DifficultySettings& GetDifficulty() {
		if (!loadedDifficulties) loadDifficulties();
		return difficultySettings[activeDifficulty];
	}

private:
	Difficulty() = delete;
	Difficulty(const Difficulty&) = delete;
	Difficulty& operator=(const Difficulty&) = delete;

	static void loadDifficulties() {
		for (int i = 0; i < difficultyCount; i++) {
			if (IO::BeginRead(string_format("assets/data/difficulty_settings/%s.txt", difficultyFiles[i].c_str()))) {
				difficultySettings[i] = {};
				IO::ReadInt(difficultySettings[i].EnemyBaseAttrPoints);
				IO::ReadInt(difficultySettings[i].EnemyAttackChance);
				IO::ReadInt(difficultySettings[i].EnemyHealChance);
				difficultySettings[i].EnemyHealChance += difficultySettings[i].EnemyAttackChance;
				IO::ReadInt(difficultySettings[i].EnemyDoNothingChance);
				IO::ReadInt(difficultySettings[i].EnemyEvadeChance);
				difficultySettings[i].EnemyDoNothingChance += difficultySettings[i].EnemyHealChance;
				IO::ReadInt(difficultySettings[i].PlayerBaseAttrPoints);
				IO::ReadInt(difficultySettings[i].PlayerFocusChance);
				IO::ReadInt(difficultySettings[i].PlayerEvadeChance);
				IO::EndRead();
			}
		}
		loadedDifficulties = true;
	}

	inline static const int difficultyCount = 3;
	inline static const std::string difficultyFiles[difficultyCount] = {"easy", "normal", "hard"};
	inline static DifficultySettings difficultySettings[difficultyCount] = {{}, {}, {}};
	inline static bool loadedDifficulties = false;
	inline static int activeDifficulty = 0;
};
