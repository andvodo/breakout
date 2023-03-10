#pragma once
#include <cstdio>
#include <map>
#include "Level.h"

class LevelManager
{
	private:
		std::map<int, Level> levels;
		const Level* getLevel(int level);
		Level* currentLevel;
		int score, level;

	public:
		LevelManager();
		bool setNextLevel();
		const Level* getCurrentLevel() const;
		void onHit(int i, int j);
		int getScore() const { return score; }
		int getLevel() const { return level; }
		bool levelCleared() const;
};
