#pragma once
#include "Ball.h"
#include "GameScreen.h"
#include "LevelManager.h"
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
		int getLives() { return lives; }
		GameState getGameState() { return gameState; }
		const LevelManager& getLevelManager() { return levelManager; };
		void onHit(int i, int j);

	private:
		static Game* instance;

		GameState gameState;
		Ball ball;
		Player player;
		GameScreen screen;
		LevelManager levelManager;

		bool playing, processingClick;
		int lives;

		bool setGame();
		void startGame();
		void processClick();
		void update();
};