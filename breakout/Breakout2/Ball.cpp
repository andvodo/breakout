#include "Ball.h"
#include "Brick.h"
#include "Game.h"
#include "Player.h"
#include "Position.h"
#include "LevelManager.h"

using namespace sf;
using namespace std;
constexpr float MAX_COORDINATE = 0.95f;
constexpr float SPEED_COEFF = 8000.f;

Ball::Ball() {
	radius = Parameters::getBallRadius();
	speed = Parameters::getBallSpeed();
	setStartPosition();
	circleShape.setFillColor(sf::Color::Cyan);
	circleShape.setRadius(radius);
	clock = Clock();
}

void Ball::setStartPosition() {
	position = Vector2f(Parameters::getWindowWidth() / 2 - radius,
		Parameters::getPlaygroundBottom() - 2 * radius - Parameters::getPlayerHeight());
	circleShape.setPosition(position);
	direction = Vector2f(1, -1);
}

void Ball::update() {
	if (!playing) {
		return;
	}

	float timeSinceLastUpdate = clock.getElapsedTime().asMicroseconds();
	clock.restart();
	float deltaX = speed * SPEED_COEFF * direction.x / (timeSinceLastUpdate * Parameters::getUpdatesPerFrame());
	float deltaY = speed * SPEED_COEFF * direction.y / (timeSinceLastUpdate * Parameters::getUpdatesPerFrame());
	position = Vector2f(position.x + deltaX, position.y + deltaY);
	circleShape.setPosition(position);
	
	const LevelManager& levelManager = Game::get()->getLevelManager();
	const Level* level = levelManager.getCurrentLevel();
	const std::vector<std::vector<Brick>>& bricks = level->getBricks();
	Brick templateBrick = bricks.at(0).at(0);
	if (position.y <= level->getRowCount() * (level->getRowSpacing() + templateBrick.getHeight())) {
		//check collision with bricks around the ball's area
		vector<int> rows;
		float rowPosition = position.y / (float)((float)level->getRowSpacing() + templateBrick.getHeight());
		rows.push_back(floor(rowPosition));
		if (position.y + radius * 2 + level->getRowSpacing() > (ceil(rowPosition)) * (templateBrick.getHeight() + level->getColumnSpacing())) {
			if (bricks.size() > ceil(rowPosition)) {
				rows.push_back(ceil(rowPosition));
			}
		}
		vector<int> columns;
		float columnPosition = position.x / (float)((float)level->getColumnSpacing() + templateBrick.getWidth());
		columns.push_back(floor(columnPosition));
		if (position.x + radius * 2 + level->getColumnSpacing() > ceil(columnPosition) * (templateBrick.getWidth() + level->getColumnSpacing())) {
			if (bricks.at(0).size() > ceil(columnPosition)) {
				columns.push_back(ceil(columnPosition));
			}
		}

		vector<std::pair<int, int>> bricksToCheck;
		for (int i = 0; i < rows.size(); i++) {
			for (int j = 0; j < columns.size(); j++) {
				int row = rows.at(i);
				int column = columns.at(j);
				if (bricks.at(row).at(column).getType() == BrickType::None) {
					continue;
				}
				bricksToCheck.emplace_back(row, column);
			}
		}
		if (bricksToCheck.size() > 0) {
			std::pair<int, int> maxDepthBrick;
			float maxDepth = 0;
			Position maxDepthPosition = Position::None;
			for (int i = 0; i < bricksToCheck.size(); i++) {
				const std::pair<int, int>& ij = bricksToCheck.at(i);
				struct CollisionInfo info = bricks.at(ij.first).at(ij.second).getCollisionInfo(position, radius);
				if (info.depth > maxDepth) {
					maxDepthBrick = ij;
					maxDepth = info.depth;
					maxDepthPosition = info.position;
				}
			}
			Game::get()->onHit(maxDepthBrick.first, maxDepthBrick.second);
			bounce(maxDepthPosition, maxDepth);
			return;
		}

		// check collision with top
		if (position.y < Parameters::getWallThickness()) {
			bounce(Position::Top, Parameters::getWallThickness());
			return;
		}
	}

	//check collision with player (angle depending on the part where the ball hit the player)
	else if (position.y + radius * 2 > Parameters::getPlaygroundBottom() - Parameters::getPlayerHeight()) {
		const Player& player = Game::get()->getPlayer();
		float playerPositionX = player.getPosition().x;
		if (position.x > playerPositionX && position.x - 2 * radius < playerPositionX + player.getWidth()) {
			float percent = (position.x - playerPositionX) / player.getWidth();
			if (direction.x > 0) {
				if (percent < 0.15) direction = Vector2f(-direction.x, -direction.y);
				else if (percent < 0.4) direction = valueForHalfAngle(-1, -1);
				else if (percent < 0.6) direction = Vector2f(direction.x, -direction.y);
				else direction = valueForDoubleAngle(1, -1);
			}
			else {
				if (percent < 0.4) direction = valueForDoubleAngle(-1, -1);
				else if (percent < 0.6) direction = Vector2f(direction.x, -direction.y);
				else if (percent < 0.85) direction = valueForHalfAngle(1, -1);
				else direction = direction = Vector2f(-direction.x, -direction.y);
			}
		}
		return;
	}

	// check collision with leftWall
	if (position.x < Parameters::getWallThickness()) {
		bounce(Position::Left, Parameters::getWallThickness());
		return;
	}
	// check collision with rightWall
	else if (position.x + radius * 2> Parameters::getWindowWidth() - Parameters::getWallThickness()) {
		bounce(Position::Right, Parameters::getWallThickness());
		return;
	}
}

Vector2f Ball::valueForDoubleAngle(int xSgn, int ySgn) {
	float angle = atan(direction.y / direction.x) / 2;
	float x = std::min(MAX_COORDINATE, abs(cos(angle)));
	float y = sqrt(1 - x * x);
	return Vector2f(x * xSgn, y * ySgn);
}

Vector2f Ball::valueForHalfAngle(int xSgn, int ySgn) {
	float angle = atan(direction.x / direction.y) / 2;
	float y = std::min(MAX_COORDINATE, abs(cos(angle)));
	float x = sqrt(1 - y * y);
	return Vector2f(x * xSgn, y * ySgn);
}

void Ball::bounce(Position hitPosition, float depth) {
	switch (hitPosition) {
		case Position::Top:
			direction = Vector2f(direction.x, -direction.y);
			position = Vector2f(position.x, position.y + depth + 1);
			break;
		case Position::Left:
			direction = Vector2f(-direction.x, direction.y);
			position = Vector2f(position.x + depth + 1, position.y);
			break;
		case Position::Right:
			direction = Vector2f(-direction.x, direction.y);
			position = Vector2f(position.x - (depth + 1), position.y);
			break;
		case Position::Bottom:
			direction = Vector2f(direction.x, -direction.y);
			position = Vector2f(position.x, position.y - (depth + 1));
			break;
		default:
			break;
	}
}

void Ball::setPlaying(bool playing) {
	this->playing = playing;
	if (!playing) {
		setStartPosition();
	}
}

Ball::~Ball() { }


