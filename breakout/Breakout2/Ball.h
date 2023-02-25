#pragma once
#include "SFML/Graphics.hpp"
#include "Parameters.h"
#include "Position.h"
#include <algorithm>

class Ball
{
	private:
		float _radius = 5;
		float _speed = 2;
		bool _playing;
		sf::Clock _clock;
		sf::Vector2f _position;
		sf::Vector2f _direction;
		sf::CircleShape _circleShape;
		void setStartPosition();
		sf::Vector2f valueForDoubleAngle(int xSgn, int ySgn);
		sf::Vector2f valueForHalfAngle(int xSgn, int ySgn);

	public:
		Ball();
		float getSpeed() { return _speed; }
		float getRadius() { return _radius; }
		sf::Vector2f getPosition() { return _position; }
		sf::CircleShape toDrawable() { return _circleShape; }
		void bounce(Position position);
		void update();
		void setPlaying(bool playing);
		~Ball();
};