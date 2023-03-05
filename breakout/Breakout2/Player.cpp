#include "Game.h"
#include "Player.h"
#include <math.h>

using namespace sf;

Player::Player() {
	width = Parameters::getWindowWidth() * Parameters::getPlayerWidthPercentage();
	height = Parameters::getPlayerHeight();
	setStartPosition();
	setPlayerAppearance();
}

void Player::setPlayerAppearance()
{
	rectangleShape.setFillColor(Color::White);
	rectangleShape.setSize(Vector2f(width, height));
}

void Player::setStartPosition()
{
	rectangleShape.setPosition(
		Vector2f(Parameters::getWindowWidth() / 2 - width / 2, Parameters::getPlaygroundBottom() - height));
}

void Player::update()
{
	if (!playing) {
		return;
	}

	float mouseX = Mouse::getPosition().x;
	float windowX = Game::get()->getGameScreen().getPosition().x;
	float x = mouseX - windowX;
	x = (x < width / 2) ? (width / 2): (std::min(x, Parameters::getWindowWidth() - width/2));
	position = Vector2f(x - width / 2, Parameters::getPlaygroundBottom() - height);
	rectangleShape.setPosition(position);
}

void Player::setPlaying(bool playing) {
	this->playing = playing;
	if (!playing) {
		setStartPosition();
	}
}

sf::Vector2f Player::getPosition() const
{
	return position;
}
