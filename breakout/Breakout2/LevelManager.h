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
		static int score;

	public:
		static bool setLevel(int level);
		static const Level* getCurrentLevel();
		static void onHit(int i, int j);
		static int getScore() { return score; }
		static bool levelCleared();
};
