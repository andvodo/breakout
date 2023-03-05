#include "Level.h"
#include "Parameters.h"
#include <sstream>

using namespace tinyxml2;

const char* LEVEL_FILE_PATH = "./Editor/Levels/Level";

Level::Level(int level)
{
	decodeLevel(level);
}

std::string Level::getLevelFilePath(int level)
{
	return LEVEL_FILE_PATH + std::to_string(level) + ".xml";
}

Level::Level(const Level& level)
{
	rowCount = level.rowCount;
	columnCount = level.columnCount;
	rowSpacing = level.rowSpacing;
	columnSpacing = level.columnSpacing;
	backgroundTexture = level.backgroundTexture;
	brickTypeData = level.brickTypeData;
	bricks = level.bricks;
}

void Level::decodeLevel(int level)
{
	XMLDocument levelDocument;
	int load = levelDocument.LoadFile(Level::getLevelFilePath(level).c_str());
	if (load != 0) {
		throw NoLevelException();
	}

	//level attributes
	XMLElement* levelElement = levelDocument.FirstChildElement("Level");
	rowCount = atoi(levelElement->Attribute("RowCount"));
	columnCount = atoi(levelElement->Attribute("ColumnCount"));
	rowSpacing = atoi(levelElement->Attribute("RowSpacing"));
	columnSpacing = atoi(levelElement->Attribute("ColumnSpacing"));
	backgroundTexture = levelElement->Attribute("BackgroundTexture");

	//brick types
	XMLElement* brickTypesElement = levelElement->FirstChildElement("BrickTypes");
	if (brickTypesElement) {
		XMLElement* brickTypeElement = brickTypesElement->FirstChildElement("BrickType");
		while (brickTypeElement) {
			decodeBrickData(brickTypeElement);
			brickTypeElement = brickTypeElement->NextSiblingElement("BrickType");
		}
	}

	//bricks
	XMLElement* bricksElement = levelElement->FirstChildElement("Bricks");
	std::string text = bricksElement->GetText();
	std::string stringToParse;
	std::stringstream sstream;
	sstream << text;
	while (getline(sstream, stringToParse)) {
		std::stringstream rowStream;
		rowStream << stringToParse;
		std::vector<Brick> row;
		while (getline(rowStream, stringToParse, ' ')) {
			if (stringToParse.empty()) {
				continue;
			}
			row.emplace_back(getBrickTypeForString(stringToParse));
		}
		if (row.size() > 0) {
			bricks.emplace_back(std::move(row)); //!!!!
		}
	}
}

void Level::decodeBrickData(const XMLElement* brickElement) {
	std::string id = brickElement->Attribute("Id");
	BrickType brickType = getBrickTypeForString(id);

	std::string texture = brickElement->Attribute("Texture");
	int hitPoints = atoi(brickElement->Attribute("HitPoints"));
	std::string hitSound = brickElement->Attribute("HitSound");

	BrickData* brickData;

	if (brickType == BrickType::Impenetrable) {
		brickData = new BrickData(texture, hitPoints, hitSound);
	}
	else {
		std::string breakSound = brickElement->Attribute("BreakSound");
		int breakScore = atoi(brickElement->Attribute("BreakScore"));

		brickData = new BrickData(texture, hitPoints, hitSound, breakSound, breakScore);
	}

	brickTypeData.emplace(brickType, *brickData);
}

BrickType Level::getBrickTypeForString(const std::string& brickTypeString)
{
	return brickTypeString == "S" ? BrickType::Soft :
			(brickTypeString == "M" ? BrickType::Medium :
				(brickTypeString == "H" ? BrickType::Hard :
					(brickTypeString == "I" ? BrickType::Impenetrable :
						BrickType::None)));
}

const BrickData& Level::getBrickDataForType(BrickType type) const
{
	auto it = brickTypeData.find(type);
	if (it != brickTypeData.end()) {
		return it->second;
	}
}

const std::vector<std::vector<Brick>>& Level::getBricks() const
{
	return bricks;
}

int Level::onHit(int row, int column)
{
	return bricks.at(row).at(column).onHit();
}

void Level::setAppearance()
{
	winScore = 0;

	float brickWidth = (Parameters::getWindowWidth() - 2 * Parameters::getWallThickness() - ((columnCount + 1) * columnSpacing))
		/ columnCount;
	float brickHeight = ((Parameters::getWindowHeight() - 2 * Parameters::getWallThickness() - Parameters::getBottomOffset())
		* Parameters::getMaxBrickArea() - ((rowCount + 1) * rowSpacing)) / rowCount;
	brickHeight = brickHeight > brickWidth / 2 ? brickWidth / 2 : brickHeight;
	for (int i = 0; i < bricks.size(); i++) {
		float yPos = Parameters::getWallThickness() + rowSpacing * (i + 1) + i * brickHeight;
		for (int j = 0; j < bricks.at(i).size(); j++) {
			float xPos = Parameters::getWallThickness() + columnSpacing * (j + 1) + j * brickWidth;
			Brick& brick = bricks.at(i).at(j);
			BrickType brickType = brick.getType();
			int breakScore = brickType == BrickType::None ? 0 : getBrickDataForType(brickType).getBreakScore();
			brick.setAppearance(brickWidth, brickHeight, xPos, yPos, breakScore);
			winScore += breakScore;
		}
	}
}