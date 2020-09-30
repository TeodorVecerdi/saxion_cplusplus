#include <sstream>
#include "Scoreboard.hpp"

template <int MaxScores>
Scoreboard<MaxScores>::Scoreboard(const char* scoreboardFilePath) {
	fileHandle = std::fstream(scoreboardFilePath);

	std::string currentLine;
	scoreCount = 0;

	// Read scores
	while (std::getline(fileHandle, currentLine)) {
		if (scoreCount >= MaxScores) break;

		std::string buffer;
		std::istringstream stream(currentLine);

		std::getline(stream, buffer, '|');
		const int currentScore = atoi(buffer.c_str());
		std::getline(stream, buffer, '|');

		scores[scoreCount] = Score{currentScore, std::move(buffer)};
		scoreCount++;
	}
}

template <int MaxScores>
Scoreboard<MaxScores>::~Scoreboard() {
	fileHandle.flush();
	fileHandle.close();
}

template <int MaxScores>
Score& Scoreboard<MaxScores>::getScore(const int scoreIndex) const {
	if (scoreIndex >= scoreCount) return invalid;
	return scores[scoreIndex];
}
