#include "LevelManager.h"
#include "tinyxml2.h"
#include <stdio.h>

int LevelManager::currentLevelNum = 0;
Level* LevelManager::currentLevel = nullptr;

Level* LevelManager::getFirstLevel() {
	currentLevelNum = 1;
	if (currentLevel != nullptr) delete currentLevel;
	currentLevel = new Level(currentLevelNum);
	return currentLevel;
}

Level* LevelManager::getNextLevel() {
	currentLevelNum++;
	if (currentLevel != nullptr) delete currentLevel;
	currentLevel = new Level(currentLevelNum);
	return currentLevel;
}

Level* LevelManager::getCurrentLevel() {
	return currentLevel;
}