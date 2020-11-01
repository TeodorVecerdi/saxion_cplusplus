#include "Scoreboard.hpp"

#include "Core/IO.hpp"

Scoreboard::Scoreboard(size_t maxScores) : maxScores(maxScores) {
	scores = new Score[maxScores];
	scoreCount = 0;

	if (!std::filesystem::exists("assets/data/highscore.txt")) {
	} else if (IO::BeginRead("assets/data/highscore.txt")) {
		int fileScoreCount = 0;
		IO::ReadInt(fileScoreCount);
		std::string currentLine;
		for (int i = 0; i < fileScoreCount; i++) {
			int difficulty, currentScore;
			std::string name;

			IO::ReadInt(difficulty);
			IO::ReadInt(currentScore);
			IO::ReadString(name);

			scores[scoreCount] = Score{difficulty, currentScore, std::move(name)};

			scoreCount++;
		}
		IO::EndRead();
	}
	sortScores();
}

Scoreboard::~Scoreboard() {
	if (scoreCount == 0) {
		if (std::filesystem::exists("assets/data/highscore.txt"))
			std::filesystem::remove("assets/data/highscore.txt");
		return;
	}
	if (IO::BeginWrite("assets/data/highscore.txt", std::ios::out | std::ios::trunc)) {
		IO::Write(scoreCount, "### Score Count");
		for (int i = 0; i < scoreCount; i++) {
			IO::WriteTitle(string_format("# %i", i));
			IO::Write(scores[i].difficulty, "Difficulty");
			IO::Write(scores[i].score, "Score");
			IO::Write(scores[i].name, "Name");
		}
		IO::EndWrite();
	}
	delete[] scores;
}

Score& Scoreboard::getScore(const int scoreIndex) const {
	if (scoreIndex < 0 || scoreIndex >= scoreCount) return InvalidScore;
	return scores[scoreIndex];
}

bool Scoreboard::addScore(Score score) {
	if (scoreCount < maxScores) {
		scores[scoreCount++] = score;
		sortScores();
		lastScoreAdded = score;
		return true;
	}

	for (int i = 0; i < scoreCount; i++) {
		if (score.score > scores[i].score) {
			scores[i] = score;
			lastScoreAdded = score;
			return true;
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

Score Scoreboard::getLastScore() const {
	return lastScoreAdded;
}

void Scoreboard::clear() {
	for (int i = 0; i < scoreCount; i++)
		scores[i] = {};
	scoreCount = 0;
}
