#pragma once
#include "SFML/Graphics.hpp"
#include "Parameters.h"
#include "Position.h"
#include <algorithm>

class Ball
{
	private:
		float radius;
		float speed;
		bool playing;
		sf::Clock clock;
		sf::Vector2f position;
		sf::Vector2f direction;
		sf::CircleShape circleShape;
		void setStartPosition();
		sf::Vector2f valueForDoubleAngle(int xSgn, int ySgn);
		sf::Vector2f valueForHalfAngle(int xSgn, int ySgn);

	public:
		Ball();
		float getSpeed() { return speed; }
		float getRadius() { return radius; }
		sf::Vector2f getPosition() { return position; }
		const sf::CircleShape& toDrawable() const { return circleShape; }
		void bounce(Position hitPosition);
		void update();
		void setPlaying(bool playing);
		~Ball();
};