#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "BrickData.h"
#include "Position.h"

class Brick
{
	private:
		BrickType brickType;
		int breakScore;
		float xPos, yPos;
		float width, height;
		void setType(BrickType brickType);
		sf::Texture texture;
		sf::Sprite sprite;
		void refresh();
		void loadTexture();

	public:
		int onHit();
		Brick(BrickType brickType);
		void setAppearance(float width, float height, float xPos, float yPos, int breakScore);
		const BrickType getType() const;
		int getBreakScore() const;
		float getWidth() { return width; }
		float getHeight() { return height; }
		const sf::Sprite& toDrawable() const;
		struct CollisionInfo getCollisionInfo(sf::Vector2f ballPosition, float radius) const;
};

struct CollisionInfo {
	float depth;
	Position position;
};