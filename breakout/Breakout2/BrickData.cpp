#include "BrickData.h"

BrickData::BrickData(std::string texture, int hitPoints, std::string hitSound, std::string breakSound, int breakScore) {
	this->texture = texture;
	this->hitPoints = hitPoints;
	this->hitSound = hitSound;
	this->breakSound = breakSound;
	this->breakScore = breakScore;
}

BrickData::BrickData(std::string texture, int hitPoints, std::string hitSound) {
	this->texture = texture;
	this->hitPoints = hitPoints;
	this->hitSound = hitSound;
	this->breakScore = 0;
}