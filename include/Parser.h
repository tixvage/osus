#pragma once
#include <vector>

struct CircleData
{
	//Circle* circle;
	float spawn_time;
	bool is_added = false;
};

std::vector<CircleData> parse();