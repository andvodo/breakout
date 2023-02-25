#include "Brick.h"
#include "LevelManager.h"

using namespace sf;
using namespace std;

Brick::Brick(BrickType brickType) {
	_brickType = brickType;
}

const BrickType Brick::getType() {
	return _brickType;
}

int Brick::getBreakScore() {
	return _breakScore;
}

void Brick::onHit() {

	switch (_brickType) {
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
	_brickType = brickType;
	refresh();
}

void Brick::setAppearance(float width, float height, float xPos, float yPos, int breakScore) {
	_xPos = xPos; _yPos = yPos;
	_width = width; _height = height;
	_breakScore = breakScore;
	if (_brickType == BrickType::None) return;

	loadTexture();

	_sprite.setPosition(xPos, yPos);
}

void Brick::loadTexture() {
	BrickData* data = LevelManager::getCurrentLevel()->getBrickDataForType(_brickType);
	std::string tex = data->getTexture();
	Image image;
	image.loadFromFile(tex);
	_texture.loadFromImage(image); //had to load from image because of a weird bug
	_sprite.setTexture(_texture);
	_sprite.setTextureRect(sf::IntRect(_xPos, _yPos, _width, _height));
}

void Brick::refresh() {
	if (_brickType == BrickType::None) _sprite.setScale(0, 0);
	else loadTexture();
}

//returns a pair with info how far into the brick did the ball go and on which side of the brick
CollisionInfo Brick::getCollisionInfo(Vector2f ballPosition, float radius) {
	struct CollisionInfo info;
	if (_brickType == BrickType::None) {
		info.depth = 0;
		info.position = Position::None;
		return info;
	}
	Level* level = LevelManager::getCurrentLevel();
	int columnSpacing = (int)level->getColumnSpacing();
	int rowSpacing = (int)level->getRowSpacing();

	if (ballPosition.x + radius > _xPos + _width / 2) {
		if (ballPosition.x + radius * 2 < _xPos + _width) {
			if (ballPosition.y + radius > _yPos + _height / 2) {
				info.depth = ballPosition.y + 2 * radius - (_yPos + _height);
				info.position = Position::Top;
			}
			else { 
				info.depth =  ballPosition.y + 2 * radius - (_yPos - rowSpacing);
				info.position = Position::Bottom;
			}
		}
		else {
			float depthFromRight = _xPos + _width - ballPosition.x;

			if (ballPosition.y + radius > _yPos + _height / 2) {
				float depthFromBottom = _yPos + _height - ballPosition.y;
				info.depth = depthFromBottom > depthFromRight ? depthFromBottom : depthFromRight;
				info.position = depthFromBottom > depthFromRight ? Position::Left : Position::Top;
			}
			else {
				float depthFromTop = ballPosition.y + radius * 2 - _yPos;
				info.depth = depthFromTop > depthFromRight ? depthFromRight : depthFromTop;
				info.position = depthFromTop > depthFromRight ? Position::Bottom : Position::Left; //***Right
			}
		}
	}
	else {
		if (ballPosition.x > _xPos) {
			if (ballPosition.y + radius > _yPos + _height / 2) {
				info.depth = _yPos + _height - ballPosition.y;
				info.position = Position::Top; 
			}
			else {
				info.depth = ballPosition.y + 2 * radius - (_yPos - rowSpacing);
				info.position = Position::Bottom; 
			}
		}
		else {
			float depthFromLeft = ballPosition.x + radius * 2 - (_xPos - columnSpacing);

			if (ballPosition.y + radius > _yPos + _height / 2) {
				float depthFromBottom = _yPos + _height - ballPosition.y;
				info.depth = depthFromBottom > depthFromLeft ? depthFromBottom : depthFromLeft;
				info.position = depthFromBottom > depthFromLeft ? Position::Right : Position::Top;
			}
			else {
				float depthFromTop = ballPosition.y + radius * 2 - (_yPos - rowSpacing);
				info.depth = depthFromTop > depthFromLeft ? depthFromTop : depthFromLeft;
				info.position = depthFromTop > depthFromLeft ? Position::Right : Position::Bottom;
			}
		}
	}
	return info;
}

Sprite Brick::toDrawable() {
	return _sprite;
}

Brick::~Brick() {}
