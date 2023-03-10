#include "Game.h"
#include "LevelManager.h"
#include "tinyxml2.h"
#include <stdio.h>

LevelManager::LevelManager()
{
	level = 0;
	score = 0;
}

const Level* LevelManager::getLevel(int level)
{
	auto it = levels.find(level - 1);
	if (it != levels.end()) {
		return &it->second;
	} else {
		try {
			levels.emplace(level - 1, Level(level));
			return &levels.at(level - 1);
		} catch (NoLevelException e) {
			return nullptr;
		}
	}
}

bool LevelManager::setNextLevel()
{
	++level;
	const Level* levelObj = getLevel(level);
	if (levelObj) {
		currentLevel = new Level(*levelObj);
		score = 0;
		currentLevel->setAppearance();
		return true;
	}
	return false;
}

const Level* LevelManager::getCurrentLevel() const
{
	return currentLevel;
}

void LevelManager::onHit(int i, int j)
{
	score += currentLevel->onHit(i, j);
}

bool LevelManager::levelCleared() const
{
	return score >= currentLevel->getWinScore();
}