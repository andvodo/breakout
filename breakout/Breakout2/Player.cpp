#include "Player.h"
#include <math.h>

using namespace sf;

Player* Player::_instance = nullptr;

Player* Player::getInstance() {
	if (_instance == nullptr) _instance = new Player();
	return _instance;
}

Player::Player() {
	_width = Parameters::getWindowWidth() * Parameters::getPlayerWidthPercentage();
	_height = Parameters::getPlayerHeight();
	setStartPosition();
	_rectangleShape.setFillColor(Color::White);
	_rectangleShape.setSize(Vector2f(_width, _height));
}

void Player::setStartPosition() {
	_position = Vector2f(Parameters::getWindowWidth() / 2 - _width / 2,
		Parameters::getWindowHeight() - Parameters::getBottomOffset() - Parameters::getPlayerHeight() - Parameters::getWallThickness());
	_rectangleShape.setPosition(_position);
}

Vector2f Player::getPosition() {
	return _position;
}

void Player::update(Vector2i windowPosition) {
	if (!_playing) return;

	float mouseX = Mouse::getPosition().x;
	float windowX = windowPosition.x;
	float x = mouseX - windowX;
	x = x < _width/2 ? _width/2 : std::min(x, Parameters::getWindowWidth() - _width/2);
	_position = Vector2f(x - _width/2, Parameters::getWindowHeight() - Parameters::getBottomOffset()
		- Parameters::getPlayerHeight() - Parameters::getWallThickness());
	_rectangleShape.setPosition(_position);
}

void Player::setPlaying(bool playing) {
	_playing = playing;
	if (!playing) setStartPosition();
}
