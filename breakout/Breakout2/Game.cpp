#include "Game.h"
#include "LevelManager.h"

using namespace sf;
using namespace std;

Game::Game() {
    _playing = false;
    _player = Player::getInstance();
    _levelNum = 0;
    _scoreNum = 0;
    _livesNum = 0;
    _gameState = State::Intro;
    _winScore = 0;

}

bool Game::initialize() {
    Level* level = LevelManager::getFirstLevel();
    _levelNum = 1;
    setGameStartParameters();
    setWalls();
    startGame();
    return EXIT_SUCCESS;
}

void Game::setGameStartParameters() {
    _livesNum = Parameters::getPlayerLives();
    _scoreNum = 0;
    _winScore = 0;
    loadBackground();
    setTexts();
    loadBricks();
}

void Game::loadBackground() {
    Level* level = LevelManager::getCurrentLevel();
    Image image;
    image.loadFromFile(level->getBackgrondTexture());
    _textureBackground.loadFromImage(image);
    _spriteBackground.setTexture(_textureBackground);
    _spriteBackground.setTextureRect(sf::IntRect(level->getColumnSpacing(), Parameters::getPlaygroundBottom(),
        Parameters::getWindowWidth() - 2 * level->getColumnSpacing(), Parameters::getPlaygroundBottom() - level->getRowSpacing()));
}

void Game::setTexts() {
    //load texts
    int fontSize = 50;
    float y = Parameters::getPlaygroundBottom() + Parameters::getBottomOffset() / 2 - fontSize / 2;
    float offsetFromWindowEdge = Parameters::getWindowHeight() - (y + fontSize);
    _font.loadFromFile("./Fonts/font.ttf");

    _text.setFont(_font);
    _text.setCharacterSize(fontSize);
    _text.setPosition(200, 200);
    if (_levelNum <= 1) _text.setString("Breakout");

    _lives.setFont(_font);
    _lives.setCharacterSize(fontSize);
    _lives.setPosition(Parameters::getWindowWidth() - fontSize - offsetFromWindowEdge, y);
    _lives.setString(to_string(_livesNum));

    _score.setFont(_font);
    _score.setCharacterSize(fontSize);
    _score.setPosition(offsetFromWindowEdge, y);
    _score.setString(to_string(_scoreNum));

    _level.setFont(_font);
    _level.setCharacterSize(fontSize);
    _level.setPosition(Parameters::getWindowWidth() / 2 + 80, y);
    _level.setString(to_string(_levelNum));

    _levelText.setFont(_font);
    _levelText.setCharacterSize(fontSize);
    _levelText.setPosition(Parameters::getWindowWidth() / 2 - 110, y);
    _levelText.setString("level");
}


void Game::setWalls() {
    _top.setPosition(0, 0);
    _top.setFillColor(Color::White);
    _top.setSize(Vector2f(Parameters::getWindowWidth(), Parameters::getWallThickness()));

    _leftWall.setPosition(0, 0);
    _leftWall.setFillColor(Color::White);
    _leftWall.setSize(Vector2f(Parameters::getWallThickness(), Parameters::getWindowHeight() - Parameters::getBottomOffset()));

    _rightWall.setPosition(Parameters::getWindowWidth() - Parameters::getWallThickness(), 0);
    _rightWall.setFillColor(Color::White);
    _rightWall.setSize(Vector2f(Parameters::getWallThickness(), Parameters::getWindowHeight() - Parameters::getBottomOffset()));

    _bottom.setPosition(0, Parameters::getWindowHeight() - Parameters::getWallThickness() - Parameters::getBottomOffset());
    _bottom.setFillColor(Color::White);
    _bottom.setSize(Vector2f(Parameters::getWindowWidth(), Parameters::getWallThickness()));
}

void Game::loadBricks() {
    _winScore = 0;
    Level* level = LevelManager::getCurrentLevel();
    vector<vector<Brick>>& bricks = level->getBricks();

    float brickWidth = (Parameters::getWindowWidth() - ((level->getColumnCount() + 1) * level->getColumnSpacing()))
    / level->getColumnCount();
    float brickHeight = ((Parameters::getWindowHeight() - Parameters::getBottomOffset()) / 2 - ((level->getRowCount() + 1) * level->getRowSpacing()))
    / level->getRowCount();
    brickHeight = brickHeight > brickWidth ? brickWidth / 2 : brickHeight;
    for (int i = 0; i < bricks.size(); i++) {
        float yPos = level->getRowSpacing() * (i + 1) + i * brickHeight;
        for (int j = 0; j < bricks.at(i).size(); j++) {
            float xPos = level->getColumnSpacing() * (j + 1) + j * brickWidth;
            Brick* brick = &bricks.at(i).at(j);
            BrickType brickType = brick->getType();
            int breakScore = brickType == BrickType::None ? 0 : level->getBrickDataForType(brickType)->getBreakScore();
            brick->setAppearance(brickWidth, brickHeight, xPos, yPos, breakScore);
            _winScore += breakScore;
        }
    }
}

void Game::startGame() {
    Clock clock;
    VideoMode videoMode = VideoMode(Parameters::getWindowWidth(), Parameters::getWindowHeight());
    _renderWindow.create(videoMode, "Breakout");
    _renderWindow.setFramerateLimit(Parameters::getFramesPerSecond());

    Clock frameClock;
    Clock updateClock;

    while (_renderWindow.isOpen())
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

        display();
        Event event;
        while (_renderWindow.pollEvent(event))
        {
            if (event.type == Event::Closed)
                _renderWindow.close();
            else
                if (event.type == Event::MouseButtonPressed)
                {
                    processClick();
                }
        }
    }
}

void Game::display() {
    _renderWindow.clear();
    _renderWindow.draw(_spriteBackground);
    _scoreNum = 0;
    vector<vector<Brick>>& bricks = LevelManager::getCurrentLevel()->getBricks();
    for (int i = 0; i < bricks.size(); i++) {
        for (int j = 0; j < bricks.at(i).size(); j++) {
            Brick* brick = &bricks.at(i).at(j);
            _renderWindow.draw(brick->toDrawable());
            if (brick->getType() == BrickType::None) _scoreNum += brick->getBreakScore();
        }
    }
    _renderWindow.draw(_top);
    _renderWindow.draw(_leftWall);
    _renderWindow.draw(_rightWall);
    _renderWindow.draw(_bottom);

    _renderWindow.draw(_ball.toDrawable());

    _renderWindow.draw(_player->toDrawable());

    if (_gameState != State::Playing) _renderWindow.draw(_text);

    _renderWindow.draw(_lives);
    _renderWindow.draw(_score);
    _renderWindow.draw(_level);
    _renderWindow.draw(_levelText);

    _renderWindow.display();
}

void Game::update() {

    if (_scoreNum >= _winScore) {
        _text.setString("Level clear");
        _ball.setPlaying(false);
        _player->setPlaying(false);
        _scoreNum = 0;

        Level* level = LevelManager::getNextLevel();
        if (level) {
            _levelNum++;
            setGameStartParameters();
            _gameState = State::LevelReady;
        }
        else {
            _gameState = State::GameWon;
            _text.setString("Game won");
        }
        return;
    }

    if (_gameState == GameWon || _gameState == GameLost) return;

    _score.setString(to_string(_scoreNum));

    _player->update(_renderWindow.getPosition());
    _ball.update();
    if (_ball.getPosition().y + _ball.getRadius() > Parameters::getPlaygroundBottom()) {
        _playing = false;
        _ball.setPlaying(false);
        _player->setPlaying(false);
        _livesNum--;
        _lives.setString(to_string(_livesNum));
        if (_livesNum > 0) {
            _gameState = State::LostLife;
            _text.setString("Life lost!");
        }
        else {
            _gameState = State::GameLost;
            _text.setString("Game over");
            LevelManager::getFirstLevel();
            setGameStartParameters();
        }
    }
}

void Game::processClick() {
    switch (_gameState) {
        case State::Intro:
            _ball.setPlaying(true);
            _player->setPlaying(true);
            _gameState = State::Playing;
            break;
        case State::Playing:
            break;
        case State::LostLife:
            _ball.setPlaying(true);
            _player->setPlaying(true);
            _gameState = State::Playing;
            break;
        case State::LevelReady:
            _ball.setPlaying(true);
            _player->setPlaying(true);
            _gameState = State::Playing;
            break;
        case State::GameLost:
            _gameState = State::Intro;
            break;
        case State::GameWon:
            _gameState = State::Intro;
            break;
        default:
            break;
    }
}

enum State {
    Intro,
    Playing,
    LevelFinished,
    LostLife,
    GameLost,
    GameWon
};

Game::~Game() {}