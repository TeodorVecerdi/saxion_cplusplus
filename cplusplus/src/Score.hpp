#pragma once
#include "Core.hpp"

struct Score {
	int score = 0;
	std::string name;
};

static Score InvalidScore {-1, ""};
