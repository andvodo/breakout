#include "BrickData.h"

BrickData::BrickData(std::string texture, int hitPoints, std::string hitSound, std::string breakSound, int breakScore) {
	_texture = texture;
	_hitPoints = hitPoints;
	_hitSound = hitSound;
	_breakSound = breakSound;
	_breakScore = breakScore;
}

BrickData::BrickData(std::string texture, int hitPoints, std::string hitSound) {
	_texture = texture;
	_hitPoints = hitPoints;
	_hitSound = hitSound;
	_breakScore = 0;
}