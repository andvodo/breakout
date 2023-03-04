#include "Game.h"
#include "LevelManager.h"
#include "tinyxml2.h"
#include <stdio.h>
#include <cassert>

Level* LevelManager::currentLevel = nullptr;
std::map<int, Level> LevelManager::levels;

const Level* LevelManager::getLevel(int level)
{
	auto it = levels.find(level);
	if (it != levels.end()) {
		return &it->second;
	} else {
		try {
			levels.emplace(level - 1, Level(level));
			return &levels.at(level - 1);
		}
		catch (NoLevelException e) {
			return nullptr;
		}
	}
}

bool LevelManager::setLevel(int level)
{
	assert(level > 0);
	const Level* levelObj = getLevel(level);
	if (levelObj) {
		currentLevel = new Level(*levelObj);
		return true;
	}
	return false;
}

Level* LevelManager::getCurrentLevel()
{
	return currentLevel;
}