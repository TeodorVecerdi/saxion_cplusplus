#pragma once
#include <string>
#include <string_view>
#include <fstream>

struct Score {
	int score;
	std::string_view name;
};

// ReSharper disable once CppNonInlineVariableDefinitionInHeaderFile
Score invalid {static_cast<int>(-1), ""};

template<int MaxScores>
class Scoreboard {
public:
	Scoreboard(const char* scoreboardFilePath);
	~Scoreboard();
	Score& getScore(int scoreIndex) const;
	int getMaxScores() const { return MaxScores; }

private:
	std::fstream& fileHandle;
	int scoreCount;
	Score scores[MaxScores];
};

