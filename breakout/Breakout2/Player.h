#pragma once

#include "SFML/Graphics.hpp"
#include "Parameters.h"

class Player
{
	private:
		sf::RectangleShape rectangleShape;
		float width, height;
		sf::Vector2f position;
		bool playing;
		void setStartPosition();
		void setPlayerAppearance();

	public:
		Player();
		float getWidth() const { return width; }
		float getHeight() const { return height; }
		void update();
		const sf::RectangleShape& toDrawable() const { return rectangleShape; }
		void setPlaying(bool playing);
		sf::Vector2f getPosition() const;
};