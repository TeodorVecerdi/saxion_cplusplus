#pragma once
#include "Core/Core.hpp"

struct Score {
	int difficulty = 0;
	int score = 0;
	std::string name;
};

static Score InvalidScore {-1, -1, ""};
