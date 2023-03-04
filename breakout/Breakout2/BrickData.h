#pragma once

#include<stdio.h>
#include<string>

class BrickData
{
private:
	int hitPoints;
	int breakScore;
	std::string texture;
	std::string hitSound;
	std::string breakSound;

public:
	BrickData(std::string texture, int hitPoints, std::string hitSound, std::string breakSound, int breakScore);
	BrickData(std::string texture, int hitPoints, std::string hitSound);
	const std::string& getTexture() const { return texture; }
	int getHitPoints() { return hitPoints; }
	const std::string& getHitSound() const { return hitSound; }
	const std::string& getBreakSound() const { return breakSound; }
	int getBreakScore() const { return breakScore; }
};

enum class BrickType {
	Soft,
	Medium,
	Hard,
	Impenetrable,
	None
};
