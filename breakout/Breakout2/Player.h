#pragma once
#include "SFML/Graphics.hpp"
#include "Parameters.h"

class Player
{
	private:
		Player();
		static Player* _instance;
		sf::RectangleShape _rectangleShape;
		float _width, _height;
		sf::Vector2f _position;
		bool _playing;
		void setStartPosition();

	public:
		static Player* getInstance();
		float getWidth() { return _width; }
		float getHeight() { return _height; }
		void update(sf::Vector2i windowPosition);
		sf::RectangleShape toDrawable() { return _rectangleShape; }
		void setPlaying(bool playing);
		sf::Vector2f getPosition();
};