#pragma once
#include "Ball.h"
#include "GameScreen.h"
#include "Parameters.h"
#include "Player.h"
#include <vector>

class Game
{
	public:
		static Game* get();
		bool initAndStart();
		const Player& getPlayer() { return player; }
		const Ball& getBall() { return ball; }
		const GameScreen& getGameScreen() { return screen; }
		int getLevel() { return level; }
		int getLives() { return lives; }
		GameState getGameState() { return gameState; }

	private:
		static Game* instance;

		GameState gameState;
		Ball ball;
		Player player;
		GameScreen screen;

		bool playing, processingClick;
		int level, lives;

		bool setGame();
		void startGame();
		void setNewLevel();
		void processClick();
		void update();
};