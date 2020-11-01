#pragma once
#include "Score.hpp"

class Scoreboard {
public:
	Scoreboard() = delete;
	Scoreboard(size_t maxScores);
	~Scoreboard();

	Score& getScore(int scoreIndex) const;
	size_t getMaxScores() const { return maxScores; }
	uint32_t getScoreCount() const { return scoreCount; }
	bool addScore(Score score);
	bool setScore(int scoreIndex, int score);
	Score getLastScore() const;
	void clear();
private:
	void sortScores();
	size_t maxScores;
	uint32_t scoreCount;
	Score* scores;
	Score lastScoreAdded = InvalidScore;
};
