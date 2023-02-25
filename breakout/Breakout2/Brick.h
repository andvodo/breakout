#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "BrickData.h"
#include "Position.h"

class Brick
{
	private:
		BrickType _brickType;
		int _breakScore;
		float _xPos, _yPos;
		float _width, _height;
		void setType(BrickType brickType);
		sf::Texture _texture;
		sf::Sprite _sprite;
		void refresh();
		void loadTexture();

	public:
		void onHit();
		Brick(BrickType brickType);
		void setAppearance(float width, float height, float xPos, float yPos, int breakScore);
		const BrickType getType();
		int getBreakScore();
		float getWidth() { return _width; }
		float getHeight() { return _height; }
		sf::Sprite toDrawable();
		struct CollisionInfo getCollisionInfo(sf::Vector2f ballPosition, float radius);
		~Brick();
};

struct CollisionInfo {
	float depth;
	Position position;
};