#pragma once
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "SFML/System.hpp"
#include "Ball.h"
#include "Player.h"
#include "Parameters.h"
#include <vector>

class Game
{
	public:
		Game();
		bool initialize();

		~Game();

		enum State {
			Intro,
			Playing,
			LevelFinished,
			LevelReady,
			LostLife,
			GameLost,
			GameWon
		};

	private:
		int _livesNum, _scoreNum, _levelNum, _winScore;
		sf::Text _text, _lives, _score, _level, _levelText;
		sf::Font _font;
		sf::Texture _textureBackground;
		sf::Sprite _spriteBackground;
		State _gameState;
		Ball _ball;
		Player* _player;
		sf::RenderWindow _renderWindow;
		sf::RectangleShape _top;
		sf::RectangleShape _leftWall;
		sf::RectangleShape _rightWall;
		sf::RectangleShape _bottom;

		bool _playing;

		void startGame();
		void setGameStartParameters();
		void processClick();
		void loadBricks();
		void loadBackground();
		void setTexts();
		void setWalls();
		void update();
		void display();
};
