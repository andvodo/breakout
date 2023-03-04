#pragma once
#include <cstdio>
#include <map>
#include "Level.h"

class LevelManager
{
	private:
		static std::map<int, Level> levels;
		static const Level* getLevel(int level);
		static Level* currentLevel;

	public:
		static bool setLevel(int level);
		static Level* getCurrentLevel();
};
