#pragma once

#include<stdio.h>
#include<string>

class BrickData
{
private:
	std::string _texture;
	int _hitPoints;
	std::string _hitSound;
	std::string _breakSound;
	int _breakScore;

public:
	BrickData(std::string texture, int hitPoints, std::string hitSound, std::string breakSound, int breakScore);
	BrickData(std::string texture, int hitPoints, std::string hitSound);
	BrickData() {}
	std::string getTexture() { return _texture; }
	int getHitPoints() { return _hitPoints; }
	std::string getHitSound() { return _hitSound; }
	std::string getBreakSound() { return _breakSound; }
	int getBreakScore() { return _breakScore; }
};

enum class BrickType {
	Soft,
	Medium,
	Hard,
	Impenetrable,
	None
};
