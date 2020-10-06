#include "Scoreboard.hpp"
#include <algorithm>

Scoreboard::Scoreboard(int maxScores, const char* scoreboardFilePath) : maxScores(maxScores), scoreboardFilePath(scoreboardFilePath) {
	scores = new Score[maxScores];
	scoreCount = 0;


	// Read scores
	fileHandle.open(scoreboardFilePath);
	std::string currentLine;
	while (std::getline(fileHandle, currentLine)) {
		if (scoreCount >= maxScores) break;
		printf("Read score line %s\n", currentLine.c_str());
		std::string buffer;
		std::istringstream stream(currentLine);

		std::getline(stream, buffer, '|');
		const int currentScore = atoi(buffer.c_str());
		std::getline(stream, buffer, '|');
		scores[scoreCount] = Score{currentScore, std::move(buffer)};
		printf("Parsed score into %i, %s\n", scores[scoreCount].score, scores[scoreCount].name.c_str());
		scoreCount++;
	}
	fileHandle.close();
}

Scoreboard::~Scoreboard() {
	std::ofstream fileHandle;
	fileHandle.open(scoreboardFilePath, std::ios::out | std::ios::trunc);
	for (int i = 0; i < scoreCount; i++) {
		printf("Adding score at index %i to file.\n", i);
		fileHandle << scores[i].score << '|' << scores[i].name << "\n";
	}
	fileHandle.close();
	delete[] scores;
}

Score& Scoreboard::getScore(const int scoreIndex) const {
	if (scoreIndex < 0 || scoreIndex >= scoreCount) return InvalidScore;
	return scores[scoreIndex];
}

bool Scoreboard::addScore(Score score, bool replaceIfInLeaderboard) {
	if (scoreCount >= maxScores && !replaceIfInLeaderboard) return false;
	if (scoreCount < maxScores) {
		scores[scoreCount++] = score;
		sortScores();
		return true;
	}

	if (replaceIfInLeaderboard) {
		for (int i = 0; i < scoreCount; i++) {
			if (score.score > scores[i].score) {
				scores[i] = score;
				return true;
			}
		}
	}

	return false;
}

bool Scoreboard::setScore(const int scoreIndex, int score) {
	if (scoreIndex < 0 || scoreIndex >= scoreCount) return false;

	scores[scoreIndex].score = score;
	sortScores();
	return true;
}

void Scoreboard::sortScores() {
	std::sort(scores, scores + scoreCount, [](const Score& a, const Score& b) { return a.score > b.score; });
}
