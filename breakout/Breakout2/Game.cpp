#include "Game.h"
#include "LevelManager.h"

using namespace sf;

Game* Game::instance = nullptr;

Game::Game() { }

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
    level = 1;

    if (!LevelManager::setLevel(level)) {
        return false;
    }

    score = 0;
    lives = Parameters::getPlayerLives();
    playing = false;
    Level* currentLevel = LevelManager::getCurrentLevel();
    currentLevel->setAppearance();
    if (gameState != GameState::GameLost) {
        screen.setAppearance();
    }
    gameState = GameState::Intro;

    return true;
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
    if (score >= LevelManager::getCurrentLevel()->getWinScore()) {
        ball.setPlaying(false);
        player.setPlaying(false);
        score = 0;

        bool levelSet = LevelManager::setLevel(level + 1);
        if (levelSet) {
            setNewLevel();
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
    if (ball.getPosition().y + ball.getRadius() > Parameters::getPlaygroundBottom()) {
        playing = false;
        ball.setPlaying(false);
        player.setPlaying(false);
        --lives;
        if (lives > 0) {
            gameState = GameState::LostLife;
        } else {
            gameState = GameState::GameLost;
        }
        screen.updateAppearance(gameState);
    }
}

void Game::setNewLevel()
{
    ++level;
    score = 0;
    LevelManager::getCurrentLevel()->setAppearance();
    screen.updateAppearance(GameState::SetNewLevel);
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

Game::~Game() {}