#include "Brick.h"
#include "Game.h"
#include "GameScreen.h"
#include "LevelManager.h"
#include <cassert>

const char* TITLE = "Breakout";
const char* GAME_OVER_MESSAGE = "GAME OVER";
const char* WIN_MESSAGE = "YOU WON!";
const char* LIFE_LOST_MESSAGE = "You lost a life!";
const int FONT_SIZE = 50;

GameScreen::GameScreen()
{
    sf::VideoMode videoMode = sf::VideoMode(Parameters::getWindowWidth(), Parameters::getWindowHeight());
    renderWindow.create(videoMode, TITLE);
    renderWindow.setFramerateLimit(Parameters::getFramesPerSecond());
}

bool GameScreen::isOpen()
{
    return renderWindow.isOpen();
}

bool GameScreen::pollEvent(sf::Event& event)
{
    return renderWindow.pollEvent(event);
}

void GameScreen::close()
{
    renderWindow.close();
}

sf::Vector2i GameScreen::getPosition() const
{
    return renderWindow.getPosition();
}

void GameScreen::display(GameState state)
{
    int level = Game::get()->getLevel();
    int score = LevelManager::getScore();

    renderWindow.clear();
    renderWindow.draw(spriteBackground);
    const std::vector<std::vector<Brick>>& bricks = LevelManager::getCurrentLevel()->getBricks();
    for (const std::vector<Brick>& brickRow: bricks) {
        for (const Brick& brick: brickRow) {
            renderWindow.draw(brick.toDrawable());
        }
    }

    renderWindow.draw(topBorder);
    renderWindow.draw(leftBorder);
    renderWindow.draw(rightBorder);
    renderWindow.draw(bottomBorder);

    if (state == GameState::GameWon || state == GameState::GameLost) {
        renderWindow.draw(messageLabel);
    } else {
        renderWindow.draw(Game::get()->getBall().toDrawable());
        renderWindow.draw(Game::get()->getPlayer().toDrawable());
    }

    renderWindow.draw(livesLabel);
    renderWindow.draw(scoreLabel);
    renderWindow.draw(levelLabel);
    renderWindow.draw(levelTextLabel);
    
    renderWindow.display();
}

void GameScreen::update()
{
    scoreLabel.setString(std::to_string(LevelManager::getScore()));
}

void GameScreen::setAppearance()
{
    setBackground();
    setWalls();
    setLabels();
}

void GameScreen::updateAppearance(GameState state)
{
    switch (state) {
    case GameState::SetNewLevel:
        livesLabel.setString(std::to_string(Game::get()->getLives()));
        levelLabel.setString(std::to_string(Game::get()->getLevel()));
        break;
    case GameState::LostLife:
        livesLabel.setString(std::to_string(Game::get()->getLives()));
        break;
    case GameState::GameLost:
        messageLabel.setString(GAME_OVER_MESSAGE);
        livesLabel.setString(std::to_string(Game::get()->getLives()));
        break;
    case GameState::Intro:
        livesLabel.setString(std::to_string(Game::get()->getLives()));
        levelLabel.setString(std::to_string(Game::get()->getLevel()));
        scoreLabel.setString(std::to_string(LevelManager::getScore()));
        break;
    case GameState::GameWon:
        messageLabel.setString(WIN_MESSAGE);
        break;
    }
}

void GameScreen::setBackground() {
    const Level* level = LevelManager::getCurrentLevel();
    sf::Image image;
    image.loadFromFile(level->getBackgroundTexture());
    textureBackground.loadFromImage(image);
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setTextureRect(sf::IntRect(0, 0,
        Parameters::getWindowWidth(), Parameters::getPlaygroundBottom()));
}

void GameScreen::setLabels()
{
    int fontSize = FONT_SIZE;
    float y = Parameters::getPlaygroundBottom() + Parameters::getBottomOffset() / 2 - fontSize / 2;
    float offsetFromWindowEdge = Parameters::getWindowHeight() - (y + fontSize);
    font.loadFromFile("./Fonts/font.ttf");

    messageLabel.setFont(font);
    messageLabel.setCharacterSize(fontSize);
    messageLabel.setPosition(200, 200);

    Game* game = Game::get();

    livesLabel.setFont(font);
    livesLabel.setCharacterSize(fontSize);
    livesLabel.setPosition(Parameters::getWindowWidth() - fontSize - offsetFromWindowEdge, y);
    livesLabel.setString(std::to_string(game->getLives()));

    scoreLabel.setFont(font);
    scoreLabel.setCharacterSize(fontSize);
    scoreLabel.setPosition(offsetFromWindowEdge, y);
    scoreLabel.setString(std::to_string(LevelManager::getScore()));

    levelLabel.setFont(font);
    levelLabel.setCharacterSize(fontSize);
    levelLabel.setPosition(Parameters::getWindowWidth() / 2 + 80, y);
    levelLabel.setString(std::to_string(game->getLevel()));

    levelTextLabel.setFont(font);
    levelTextLabel.setCharacterSize(fontSize);
    levelTextLabel.setPosition(Parameters::getWindowWidth() / 2 - 110, y);
    levelTextLabel.setString("level");
}


void GameScreen::setWalls() {
    topBorder.setPosition(0, 0);
    topBorder.setFillColor(sf::Color::White);
    topBorder.setSize(sf:: Vector2f(Parameters::getWindowWidth(), Parameters::getWallThickness()));

    leftBorder.setPosition(0, 0);
    leftBorder.setFillColor(sf::Color::White);
    leftBorder.setSize(sf::Vector2f(Parameters::getWallThickness(), Parameters::getWindowHeight() - Parameters::getBottomOffset()));

    rightBorder.setPosition(Parameters::getWindowWidth() - Parameters::getWallThickness(), 0);
    rightBorder.setFillColor(sf::Color::White);
    rightBorder.setSize(sf::Vector2f(Parameters::getWallThickness(), Parameters::getWindowHeight() - Parameters::getBottomOffset()));

    bottomBorder.setPosition(0, Parameters::getWindowHeight() - Parameters::getWallThickness() - Parameters::getBottomOffset());
    bottomBorder.setFillColor(sf::Color::White);
    bottomBorder.setSize(sf::Vector2f(Parameters::getWindowWidth(), Parameters::getWallThickness()));
}