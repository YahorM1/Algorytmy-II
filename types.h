#pragma once

#include<tuple>
#include<unordered_map>


enum class MineTypes { gold = 1, copper, coal, iron, silver };
enum class ZeroMark { notmarked = 0, starred, primed };


struct mine_t
{
	MineTypes type;
	std::pair<float, float> position;
};

struct dwarf_t {
	std::pair<float, float> position;
	std::unordered_map<MineTypes, float> skills;
};

