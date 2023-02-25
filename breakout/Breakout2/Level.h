#pragma once

#include<stdio.h>
#include<string>
#include<vector>
#include <sstream>
#include<map>
#include "tinyxml2.h"
#include "Brick.h"

class Level
{
	private:
		int _rowCount;
		int _columnCount;
		float _rowSpacing;
		float _columnSpacing;
		std::string _backgrondTexture;
		const char* _filePath = "./Editor/Levels/Level";
		std::map<BrickType, BrickData> _brickTypeData;
		std::vector<std::vector<Brick>> _bricks;

		std::string getLevelFilePath(int level);
		void DecodeLevel(int level);
		void DecodeBrickData(tinyxml2::XMLElement* brickElement);
		BrickType getBrickTypeForString(std::string str);

	public:
		Level(int level);
		~Level();

		int getRowCount() { return _rowCount; }
		int getColumnCount() { return _columnCount; }
		float getRowSpacing() { return _rowSpacing; }
		float getColumnSpacing() { return _columnSpacing; }
		std::string getBackgrondTexture() { return _backgrondTexture; }

		BrickData* getBrickDataForType(BrickType type);
		std::vector<std::vector<Brick>>& getBricks();
};