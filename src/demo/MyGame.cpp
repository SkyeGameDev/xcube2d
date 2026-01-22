#include "MyGame.h"

MyGame::MyGame() : AbstractGame(), score(0), lives(3), numKeys(28), gameWon(false), box(5, 5, 30, 30), level(1) {
    TTF_Font * font = ResourceManager::loadFont("res/fonts/arial.ttf", 72);
    gfx->useFont(font);
    gfx->setVerticalSync(true);

    LevelFile level01 = mySystem->loadLevel("templateMap/room01", 1, 30, 1110, 750);

    box.x = level01.getGrid().tiles[1][0][0];
    box.y = level01.getGrid().tiles[1][0][1];

    numKeys = level01.getGrid().tiles[2].size();

    for (int i = 0; i < numKeys; i++) {
        std::shared_ptr<GameKey> k = std::make_shared<GameKey>();
        k->isAlive = true;
        k->pos = Point2(level01.getGrid().tiles[2][i][0], level01.getGrid().tiles[2][i][1]);
        gameKeys.push_back(k);
    }
}

MyGame::~MyGame() { }

void MyGame::handleKeyEvents() {
    int speed = 3;

    if (eventSystem->isPressed(Key::W)) {
        velocity.y = -speed;
    }

    if (eventSystem->isPressed(Key::S)) {
        velocity.y = speed;
    }

    if (eventSystem->isPressed(Key::A)) {
        velocity.x = -speed;
    }

    if (eventSystem->isPressed(Key::D)) {
        velocity.x = speed;
    }
}

void MyGame::update() {
    box.x += velocity.x;
    box.y += velocity.y;

    for (auto key : gameKeys) {
        if (key->isAlive && box.contains(key->pos)) {
            score += 200;
            key->isAlive = false;
            numKeys--;
        }
    }
    
    velocity.x = 0;
    velocity.y = 0;

    if (numKeys == 0) {
        gameWon = true;
        levelTwo();
    }
}

void MyGame::levelTwo() {
    level++;
    
    if (level == 2) {
        LevelFile level02 = mySystem->loadLevel("templateMap/room02", 1, 20, 740, 500);

        box.x = level02.getGrid().tiles[1][0][0];
        box.y = level02.getGrid().tiles[1][0][1];

        numKeys = level02.getGrid().tiles[2].size();

        for (int i = 0; i < numKeys; i++) {
            std::shared_ptr<GameKey> k = std::make_shared<GameKey>();
            k->isAlive = true;
            k->pos = Point2(level02.getGrid().tiles[2][i][0], level02.getGrid().tiles[2][i][1]);
            gameKeys.push_back(k);
        }

        gameWon = false;
    }
}

void MyGame::render() {
    gfx->setDrawColor(SDL_COLOR_RED);
    gfx->drawRect(box);

    gfx->setDrawColor(SDL_COLOR_YELLOW);
    for (auto key : gameKeys)
        if (key->isAlive)
            gfx->drawCircle(key->pos, 5);
}

void MyGame::renderUI() {
    gfx->setDrawColor(SDL_COLOR_AQUA);
    std::string scoreStr = std::to_string(score);
    gfx->drawText(scoreStr, 780 - scoreStr.length() * 50, 25);

    if (gameWon)
        gfx->drawText("YOU WON", 250, 500);
}
