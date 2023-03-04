#include "Brick.h"
#include "Game.h"
#include "LevelManager.h"

using namespace sf;
using namespace std;

Brick::Brick(BrickType brickType) {
	this->brickType = brickType;
}

const BrickType Brick::getType() const
{
	return brickType;
}

int Brick::getBreakScore() const
{
	return breakScore;
}

void Brick::onHit() {

	switch (brickType) {
		case BrickType::Impenetrable:
			break;
		case BrickType::Hard:
			setType(BrickType::Medium);
			break;
		case BrickType::Medium:
			setType(BrickType::Soft);
			break;
		case BrickType::Soft:
			setType(BrickType::None);
			break;
		default:
			break;
	}
}

void Brick::setType(BrickType brickType) {
	this->brickType = brickType;
	refresh();
}

void Brick::setAppearance(float width, float height, float xPos, float yPos, int breakScore) {
	xPos = xPos;
	yPos = yPos;
	width = width;
	height = height;
	breakScore = breakScore;

	if (brickType == BrickType::None) {
		return;
	}

	loadTexture();
	sprite.setPosition(xPos, yPos);
}

void Brick::loadTexture() {
	const BrickData& data = LevelManager::getCurrentLevel()->getBrickDataForType(brickType);
	std::string tex = data.getTexture();
	Image image;
	image.loadFromFile(tex);
	texture.loadFromImage(image); //had to load from image because of a weird bug
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(xPos, yPos, width, height));
}

void Brick::refresh() {
	if (brickType == BrickType::None) {
		sprite.setScale(0, 0);
	} else {
		loadTexture();
	}
}

//returns a pair with info how far into the brick did the ball go and on which side of the brick
CollisionInfo Brick::getCollisionInfo(Vector2f ballPosition, float radius) const {
	struct CollisionInfo info;
	if (brickType == BrickType::None) {
		info.depth = 0;
		info.position = Position::None;
		return info;
	}
	const Level* level = LevelManager::getCurrentLevel();
	int columnSpacing = level->getColumnSpacing();
	int rowSpacing = level->getRowSpacing();

	if (ballPosition.x + radius > xPos + width / 2) {
		if (ballPosition.x + radius * 2 < xPos + width) {
			if (ballPosition.y + radius > yPos + height / 2) {
				info.depth = ballPosition.y + 2 * radius - (yPos + height);
				info.position = Position::Top;
			} else { 
				info.depth =  ballPosition.y + 2 * radius - (yPos - rowSpacing);
				info.position = Position::Bottom;
			}
		}
		else {
			float depthFromRight = xPos + width - ballPosition.x;

			if (ballPosition.y + radius > yPos + height / 2) {
				float depthFromBottom = yPos + height - ballPosition.y;
				info.depth = depthFromBottom > depthFromRight ? depthFromBottom : depthFromRight;
				info.position = depthFromBottom > depthFromRight ? Position::Left : Position::Top;
			}
			else {
				float depthFromTop = ballPosition.y + radius * 2 - yPos;
				info.depth = depthFromTop > depthFromRight ? depthFromRight : depthFromTop;
				info.position = depthFromTop > depthFromRight ? Position::Bottom : Position::Left; //***Right
			}
		}
	}
	else {
		if (ballPosition.x > xPos) {
			if (ballPosition.y + radius > yPos + height / 2) {
				info.depth = yPos + height - ballPosition.y;
				info.position = Position::Top; 
			}
			else {
				info.depth = ballPosition.y + 2 * radius - (yPos - rowSpacing);
				info.position = Position::Bottom; 
			}
		}
		else {
			float depthFromLeft = ballPosition.x + radius * 2 - (xPos - columnSpacing);

			if (ballPosition.y + radius > yPos + height / 2) {
				float depthFromBottom = yPos + height - ballPosition.y;
				info.depth = depthFromBottom > depthFromLeft ? depthFromBottom : depthFromLeft;
				info.position = depthFromBottom > depthFromLeft ? Position::Right : Position::Top;
			}
			else {
				float depthFromTop = ballPosition.y + radius * 2 - (yPos - rowSpacing);
				info.depth = depthFromTop > depthFromLeft ? depthFromTop : depthFromLeft;
				info.position = depthFromTop > depthFromLeft ? Position::Right : Position::Bottom;
			}
		}
	}
	return info;
}

const Sprite& Brick::toDrawable() const {
	return sprite;
}