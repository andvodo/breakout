#pragma once
#include "Level.h"

class LevelManager
{
	private:
		static int currentLevelNum;
		static Level* currentLevel;
		const std::string levelFilePath = "Editor/Levels/Level";

	public:
		static Level* getFirstLevel();
		static Level* getNextLevel();
		static Level* getCurrentLevel();
};
