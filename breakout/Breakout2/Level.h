#pragma once

#include <cstdio>
#include <map>
#include <string>
#include <vector>
#include "Brick.h"
#include "tinyxml2.h"

class NoLevelException {};

class Level
{
	private:
		int rowCount, columnCount, winScore;
		float rowSpacing, columnSpacing;
		std::string backgroundTexture;
		std::map<BrickType, BrickData> brickTypeData;
		std::vector<std::vector<Brick>> bricks;

		std::string getLevelFilePath(int level);
		void decodeLevel(int level);
		void decodeBrickData(const tinyxml2::XMLElement* brickElement);
		BrickType getBrickTypeForString(const std::string& brickTypeString);

	public:
		Level(int level);
		Level(const Level& level);
		Level(Level&& level) = default;

		int getRowCount() const { return rowCount; }
		int getColumnCount() const { return columnCount; }
		float getRowSpacing() const { return rowSpacing; }
		float getColumnSpacing() const { return columnSpacing; }
		int getWinScore() const { return winScore; }
		const std::string& getBackgroundTexture() const { return backgroundTexture; }

		const BrickData& getBrickDataForType(BrickType type) const;
		const std::vector<std::vector<Brick>>& getBricks() const;
		int onHit(int row, int column);
		void setAppearance();
};