#pragma once

#include "GameState.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class GameScreen
{
public:
	GameScreen();
	void display(GameState state);
	bool isOpen();
	bool pollEvent(sf::Event& event);
	void close();
	sf::Vector2i getPosition() const;
	void setAppearance();
	void updateAppearance(GameState state);

private:
	sf::RenderWindow renderWindow;
	sf::RectangleShape topBorder;
	sf::RectangleShape leftBorder;
	sf::RectangleShape rightBorder;
	sf::RectangleShape bottomBorder;
	sf::Text messageLabel, livesLabel, scoreLabel, levelLabel, levelTextLabel;
	sf::Font font;
	sf::Texture textureBackground;
	sf::Sprite spriteBackground;

	void setBackground();
	void setLabels();
	void setWalls();
};