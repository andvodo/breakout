#include "Level.h"

using namespace tinyxml2;
using namespace std;

Level::Level(int level) {
	Level::DecodeLevel(level);
}

std::string Level::getLevelFilePath(int level) {
	stringstream ss;
	ss << _filePath << level << ".xml";
	string path;

	ss >> path;
	return path;
}

void Level::DecodeLevel(int level) {
	XMLDocument levelDocument;
	int i = levelDocument.LoadFile(Level::getLevelFilePath(level).c_str());

	//level attributes
	XMLElement* levelElement = levelDocument.FirstChildElement("Level");
	_rowCount = atoi(levelElement->Attribute("RowCount"));
	_columnCount = atoi(levelElement->Attribute("ColumnCount"));
	_rowSpacing = atoi(levelElement->Attribute("RowSpacing"));
	_columnSpacing = atoi(levelElement->Attribute("ColumnSpacing"));
	_backgrondTexture = levelElement->Attribute("BackgroundTexture");

	//brick types
	XMLElement* brickTypesElement = levelElement->FirstChildElement("BrickTypes");
	if (brickTypesElement) {
		XMLElement* brickTypeElement = brickTypesElement->FirstChildElement("BrickType");
		while (brickTypeElement) {
			DecodeBrickData(brickTypeElement);
			brickTypeElement = brickTypeElement->NextSiblingElement("BrickType");
		}
	}

	//bricks
	XMLElement* bricksElement = levelElement->FirstChildElement("Bricks");
	string text = bricksElement->GetText();
	string stringToParse;
	stringstream sstream;
	sstream << text;
	while (getline(sstream, stringToParse)) {
		stringstream rowStream;
		rowStream << stringToParse;
		vector<Brick> row = vector<Brick>();
		while (getline(rowStream, stringToParse, ' ')) {
			if (stringToParse == "") continue;
			Brick brick = Brick(getBrickTypeForString(stringToParse));
			row.push_back(brick);
		}
		if (row.size() > 0) _bricks.push_back(row);
	}
}

void Level::DecodeBrickData(XMLElement* brickElement) {
	string id = brickElement->Attribute("Id");
	BrickType brickType = getBrickTypeForString(id);

	string texture = brickElement->Attribute("Texture");
	int hitPoints = atoi(brickElement->Attribute("HitPoints"));
	string hitSound = brickElement->Attribute("HitSound");

	BrickData* brickData;

	if (brickType == BrickType::Impenetrable) {
		brickData = new BrickData(texture, hitPoints, hitSound);
	}
	else {
		string breakSound = brickElement->Attribute("BreakSound");
		int breakScore = atoi(brickElement->Attribute("BreakScore"));

		brickData = new BrickData(texture, hitPoints, hitSound, breakSound, breakScore);
	}

	_brickTypeData[brickType] = *brickData;
}

BrickType Level::getBrickTypeForString(std::string str) {
	return str == "S" ? BrickType::Soft :
		(str == "M" ? BrickType::Medium : (str == "H" ? BrickType::Hard :
			(str == "I" ? BrickType::Impenetrable : BrickType::None)));
}

BrickData* Level::getBrickDataForType(BrickType type) {
	if (_brickTypeData.find(type) != _brickTypeData.end()) {
		return &(_brickTypeData.at(type));
	}
}

vector<vector<Brick>>& Level::getBricks() {
	return _bricks;
}

Level::~Level() {
}