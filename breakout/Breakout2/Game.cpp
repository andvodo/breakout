#include "Game.h"

using namespace sf;

Game* Game::instance = nullptr;

Game* Game::get()
{
    if (!instance) {
        instance = new Game();
    }
    return instance;
}

bool Game::initAndStart()
{
    if (setGame()) {
        startGame();
        return true;
    }
    return false;
}

bool Game::setGame() {
    if (!levelManager.setNextLevel()) {
        return false;
    }

    lives = Parameters::getPlayerLives();
    playing = false;
    if (gameState != GameState::GameLost) {
        screen.setAppearance();
    }
    else {
        screen.updateAppearance(GameState::Intro);
    }
    gameState = GameState::Intro;

    return true;
}

void Game::onHit(int i, int j)
{
    levelManager.onHit(i, j);
}

void Game::startGame() {
    Clock clock;
    Clock frameClock;
    Clock updateClock;

    while (screen.isOpen())
    {
        frameClock.restart();
        int frameTime = 1000000 / Parameters::getFramesPerSecond();
        int updatesPerFrame = Parameters::getUpdatesPerFrame();
        float updateTime = frameTime / updatesPerFrame;
        int updateCnt = 0;
        updateClock.restart();
        do {
            if (frameClock.getElapsedTime().asMicroseconds() >= frameTime) break;
            if (updateClock.getElapsedTime().asMicroseconds() < updateTime) continue;

            update();
            updateClock.restart();
        } while (true);

        screen.display(gameState);
        Event event;
        while (screen.pollEvent(event)) {
            if (event.type == Event::Closed) {
                screen.close();
            } else if (event.type == Event::MouseButtonPressed) {
                processClick();
            }
        }
    }
}

void Game::update()
{
    if (levelManager.levelCleared()) {
        ball.setPlaying(false);
        player.setPlaying(false);

        bool levelSet = levelManager.setNextLevel();
        if (levelSet) {
            screen.updateAppearance(GameState::SetNewLevel);
            gameState = GameState::LevelReady;
        }
        else {
            gameState = GameState::GameWon;
            screen.updateAppearance(gameState);
        }
        return;
    }

    if (gameState == GameState::GameWon || gameState == GameState::GameLost) {
        return;
    }

    player.update();
    ball.update();
    screen.update();
    if (ball.getPosition().y + ball.getRadius() * 2 > Parameters::getPlaygroundBottom()) {
        playing = false;
        ball.setPlaying(false);
        player.setPlaying(false);
        --lives;
        if (lives > 0) {
            gameState = GameState::LostLife;
        } else {
            gameState = GameState::GameLost;
            screen.updateAppearance(gameState);
        }
        screen.updateAppearance(gameState);
    }
}

void Game::processClick()
{
    if (processingClick) {
        return;
    }

    processingClick = true;

    switch (gameState) {
        case GameState::Playing:
            break;
        case GameState::Intro:
        case GameState::LostLife:
        case GameState::LevelReady:
            ball.setPlaying(true);
            player.setPlaying(true);
            gameState = GameState::Playing;
            break;
        case GameState::GameLost:
        case GameState::GameWon:
            setGame();
            break;
        default:
            break;
    }
    processingClick = false;
}