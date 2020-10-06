#pragma once
#include <fstream>
#include <sstream>
#include "Score.hpp"

class Scoreboard {
public:
	Scoreboard() = delete;
	Scoreboard(int maxScores, const char* scoreboardFilePath);
	~Scoreboard();
	
	Score& getScore(int scoreIndex) const;
	constexpr size_t getMaxScores() const { return maxScores; }
	bool addScore(Score score, bool replaceIfInLeaderboard = false);
	bool setScore(int scoreIndex, int score);
private:
	void sortScores();
private:
	std::ifstream fileHandle;
	std::string scoreboardFilePath;
	int maxScores;
	int scoreCount;
	Score* scores;
};
