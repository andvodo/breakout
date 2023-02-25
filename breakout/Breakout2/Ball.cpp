#include "Ball.h"
#include "Brick.h"
#include "Player.h"
#include "Position.h"
#include "LevelManager.h"

using namespace sf;
using namespace std;
constexpr float maxCoordinate = 0.95;
constexpr float bounceDelta = 2;
constexpr float speedCoef = 8000;

Ball::Ball() {
	setStartPosition();
	_circleShape.setFillColor(sf::Color::Cyan);
	_circleShape.setRadius(_radius);
	_clock = Clock();
}

void Ball::setStartPosition() {
	_position = Vector2f(Parameters::getWindowWidth() / 2 - _radius,
		Parameters::getWindowHeight() - Parameters::getBottomOffset()
		- 2 * _radius - Parameters::getWallThickness() - Parameters::getPlayerHeight());
	_circleShape.setPosition(_position);
	_direction = Vector2f(1, -1);
}

void Ball::update() {
	if (!_playing) return;

	float ms = _clock.getElapsedTime().asMicroseconds();
	_clock.restart();
	float deltaX = _speed * _direction.x / ms / Parameters::getUpdatesPerFrame() * speedCoef;
	float deltaY = _speed * _direction.y / ms / Parameters::getUpdatesPerFrame() * speedCoef;
	_position = Vector2f(_position.x + deltaX, _position.y + deltaY);
	_circleShape.setPosition(_position);
	
	Level* level = LevelManager::getCurrentLevel();
	vector<vector<Brick>>& bricks = level->getBricks();
	Brick templateBrick = bricks.at(0).at(0);
	if (_position.y <= level->getRowCount() * (level->getRowSpacing() + templateBrick.getHeight())) {
		//check collision with bricks around the ball's area
		vector<int> rows;
		float row = _position.y / (float)((float)level->getRowSpacing() + templateBrick.getHeight());
		rows.push_back(floor(row));
		if (_position.y + _radius * 2 + level->getRowSpacing() > (ceil(row)) * (templateBrick.getHeight() + level->getColumnSpacing())) {
			if (bricks.size() > ceil(row)) rows.push_back(ceil(row));
		}
		vector<int> columns;
		float column = _position.x / (float)((float)level->getColumnSpacing() + templateBrick.getWidth());
		columns.push_back(floor(column));
		if (_position.x + _radius * 2 + level->getColumnSpacing() > ceil(column) * (templateBrick.getWidth() + level->getColumnSpacing())) {
			if (bricks.at(0).size() > ceil(column)) columns.push_back(ceil(column));
		}

		vector<Brick*> bricksToCheck;
		for (int i = 0; i < rows.size(); i++) {
			for (int j = 0; j < columns.size(); j++) {
				Brick* brick = &bricks.at(rows.at(i)).at(columns.at(j));
				if (brick->getType() == BrickType::None) continue;
				bricksToCheck.push_back(brick);
			}
		}
		if (bricksToCheck.size() > 0) {
			int maxDepthBrick = 0;
			float maxDepth = 0;
			Position maxDepthPosition = Position::None;
			for (int i = 0; i < bricksToCheck.size(); i++) {
				struct CollisionInfo info = bricksToCheck.at(i)->getCollisionInfo(_position, _radius);
				if (info.depth > maxDepth) {
					maxDepthBrick = i;
					maxDepth = info.depth;
					maxDepthPosition = info.position;
				}
			}
			bricksToCheck.at(maxDepthBrick)->onHit();
			bounce(maxDepthPosition);
		}

		// check collision with top
		if (_position.y < Parameters::getWallThickness()) {
			bounce(Position::Top);
			return;
		}
	}

	//check collision with player (angle depending on the part where the ball hit the player)
	else if (_position.y + _radius * 2 > Parameters::getPlaygroundBottom() - Parameters::getPlayerHeight()) {
		Player* player = Player::getInstance();
		float playerPositionX = player->getPosition().x;
		if (_position.x > playerPositionX && _position.x < playerPositionX + player->getWidth()) {
			float percent = (_position.x - playerPositionX) / player->getWidth();
			if (_direction.x > 0) {
				if (percent < 0.15) _direction = Vector2f(-_direction.x, -_direction.y);
				else if (percent < 0.4) _direction = valueForHalfAngle(-1, -1);
				else if (percent < 0.6) _direction = Vector2f(_direction.x, -_direction.y);
				else _direction = valueForDoubleAngle(1, -1);
			}
			else {
				if (percent < 0.4) _direction = valueForDoubleAngle(-1, -1);
				else if (percent < 0.6) _direction = Vector2f(_direction.x, -_direction.y);
				else if (percent < 0.85) _direction = valueForHalfAngle(1, -1);
				else _direction = _direction = Vector2f(-_direction.x, -_direction.y);
			}
		}
		return;
	}

	// check collision with leftWall
	if (_position.x < Parameters::getWallThickness()) {
		bounce(Position::Left);
		return;
	}
	// check collision with rightWall
	else if (_position.x + _radius * 2> Parameters::getWindowWidth() - Parameters::getWallThickness()) {
		bounce(Position::Right);
		return;
	}
}

Vector2f Ball::valueForDoubleAngle(int xSgn, int ySgn) {
	float angle = atan(_direction.y / _direction.x) / 2;
	float x = std::min(maxCoordinate, abs(cos(angle)));
	float y = sqrt(1 - x * x);
	return Vector2f(x * xSgn, y * ySgn);
}

Vector2f Ball::valueForHalfAngle(int xSgn, int ySgn) {
	float angle = atan(_direction.x / _direction.y) / 2;
	float y = std::min(maxCoordinate, abs(cos(angle)));
	float x = sqrt(1 - y * y);
	return Vector2f(x * xSgn, y * ySgn);
}

void Ball::bounce(Position position) {
	switch (position) {
		case Position::Top:
			_direction = Vector2f(_direction.x, -_direction.y);
			_position = Vector2f(_position.x, _position.y + bounceDelta);
			break;
		case Position::Left:
			_direction = Vector2f(-_direction.x, _direction.y);
			_position = Vector2f(_position.x + bounceDelta, _position.y);
			break;
		case Position::Right:
			_direction = Vector2f(-_direction.x, _direction.y);
			_position = Vector2f(_position.x - bounceDelta, _position.y);
			break;
		case Position::Bottom:
			_direction = Vector2f(_direction.x, -_direction.y);
			_position = Vector2f(_position.x, _position.y - bounceDelta);
			break;
		default:
			break;
	}
}

void Ball::setPlaying(bool playing) {
	_playing = playing;
	if (!playing) setStartPosition();
}

Ball::~Ball() { }


