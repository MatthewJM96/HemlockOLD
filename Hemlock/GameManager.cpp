#include "GameManager.h"

#include "Gun.h"

#include <Xylem\Xylem.h>
#include <Xylem\ResourceManager.h>

#include <SDL\SDL.h>
#include <iostream>

GameManager::GameManager()
    : _screenWidth(1280), _screenHeight(720), _gameState(GameState::PLAY), _fps(60.0f), _maxFPS(90.0f), _player(nullptr), _levelID(1)
{
    _camera.init(_screenWidth, _screenHeight);
}

GameManager::~GameManager() 
{
    // IMPLEMENT THIS!
}

void GameManager::run() 
{
    initSystems();

    gameLoop();
}

void GameManager::initSystems()
{
    Xylem::init();

    _fpsLimiter.init(_maxFPS);

    _window.create("Zombie Simulator 2015", _screenWidth, _screenHeight, 0);

    constructLevel(0);

    initShaders();

    _spriteBatch.init();

    _level.init();
}

void GameManager::initShaders()
{
    // Compile our texture shader
    _textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    _textureProgram.addAttribute("vertexPosition");
    _textureProgram.addAttribute("vertexColour");
    _textureProgram.addAttribute("vertexUV");
    _textureProgram.linkShaders();
}

void GameManager::gameLoop()
{
    constructLevel(_levelID);

    static int frameCount = 0;
    while (_gameState != GameState::EXIT && _gameState != GameState::EXIT_CONFIRMATION) {
        _fpsLimiter.begin();

        if (_zombies.size() == 0) {
            constructLevel(++_levelID);
        }

        processInput();

        _camera.update();

        _player->update(_inputManager, _bullets, _camera);

        _level.collideEntity(*_player);

        // Update bullets.
        for (size_t i = 0; i < _bullets.size();) {
            // Update bullets.
            if (_bullets[i]->update() == false) {
                goto DESTROY_BULLET;
            }
            // Collide with zombies.
            for (size_t j = 0; j < _zombies.size(); ++j) {
                if (_bullets[i]->collideWithEntity(*(_zombies[j]))) {
                    _zombies[j]->changeHealth(-1.0f * _bullets[i]->getDamage());
                    goto DESTROY_BULLET;
                }
            }
            // Collide with civilians.
            for (size_t j = 0; j < _civilians.size(); ++j) {
                if (_bullets[i]->collideWithEntity(*(_civilians[j]))) {
                    _civilians[j]->changeHealth(-1.0f * _bullets[i]->getDamage());
                    goto DESTROY_BULLET;
                }
            }
            ++i;
            continue;
        DESTROY_BULLET:
            _bullets[i] = _bullets.back();
            _bullets.pop_back();
        }

        // Handle updating Civilians.
        for (size_t i = 0; i < _civilians.size();) {
            // Collide with player.
            _civilians[i]->collideWithEntity(*_player);
            // Collide with civilians.
            for (size_t j = i + 1; j < _civilians.size(); ++j) {
                _civilians[i]->collideWithEntity(*(_civilians[j]));
            }
            // Update civilian location.
            if (_civilians[i]->update() == false) {
                _civilians[i] = _civilians.back();
                _civilians.pop_back();
            } else {
                ++i;
            }
        }

        for (size_t i = 0; i < _zombies.size();) {
            // Collide with civilians.
            for (size_t j = 0; j < _civilians.size();) {
                if (_zombies[i]->collideWithEntity(*(_civilians[j]))) {
                    glm::vec2 civilianLocation = _civilians[j]->getPosition();
                    _civilians[j] = _civilians.back();
                    _civilians.pop_back();
                    _zombies.push_back(new Zombie(glm::vec2(0.0f, 0.0f), civilianLocation));
                }
                else {
                    ++j;
                }
            }
            // Collide with player.
            if (_zombies[i]->collideWithEntity(*_player)) {
                std::cout << "Game over!" << std::endl;
                _gameState = GameState::EXIT_CONFIRMATION;
            }
            // Collide with zombie.
            for (size_t j = i + 1; j < _zombies.size(); ++j) {
                _zombies[i]->collideWithEntity(*(_zombies[j]));
            }
            // Update zombie.
            if (_zombies[i]->update(_player, _civilians) == false) {
                _zombies[i] = _zombies.back();
                _zombies.pop_back();
            } else {
                ++i;
            }
        }

        drawGame();

        _fps = _fpsLimiter.end();
        if (++frameCount % 60 == 0) {
            std::cout << _fps << std::endl;
            // Also calculate and display entity count.
            int entityCount = 1;
            entityCount += _zombies.size();
            entityCount += _civilians.size();
            entityCount += _bullets.size();
            std::cout << entityCount << std::endl;
        }
    }

    if (_gameState == GameState::EXIT_CONFIRMATION) {
        int temp;
        std::cout << "Press any key to exit..." << std::endl;
        std::cin >> temp;
    }
}

void GameManager::processInput()
{
    const float CAMERA_SPEED = 1.3f;
    const float SCALE_SPEED = 0.1f;

    SDL_Event evnt;
    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        glm::vec2 worldCoords;
        switch (evnt.type) {
            case SDL_QUIT:
                _gameState = GameState::EXIT;
                break;
            case SDL_MOUSEMOTION:
                _inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                _inputManager.pressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                _inputManager.releaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                _inputManager.pressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                _inputManager.releaseKey(evnt.button.button);
                break;
        }
    }

    if (_inputManager.isKeyPressed(SDLK_w)) {
        _camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
    }
    if (_inputManager.isKeyPressed(SDLK_s)) {
        _camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -1.0f * CAMERA_SPEED));
    }
    if (_inputManager.isKeyPressed(SDLK_a)) {
        _camera.setPosition(_camera.getPosition() + glm::vec2(-1.0f * CAMERA_SPEED, 0.0f));
    }
    if (_inputManager.isKeyPressed(SDLK_d)) {
        _camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
    }
    if (_inputManager.isKeyPressed(SDLK_q)) {
        _camera.setScale(_camera.getScale() + SCALE_SPEED);
    }
    if (_inputManager.isKeyPressed(SDLK_e)) {
        _camera.setScale(_camera.getScale() - SCALE_SPEED);
    }
}

void GameManager::drawGame() 
{
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _textureProgram.use();

    glActiveTexture(GL_TEXTURE0);

    GLint textureLocation = _textureProgram.getUniformLocation("tex");
    glUniform1i(textureLocation, 0);

    GLuint pLocation = _textureProgram.getUniformLocation("P");
    glm::mat4 cameraMatrix = _camera.getCameraMatrix();
    glUniformMatrix4fv(pLocation, 1, false, &(cameraMatrix[0][0]));

    _spriteBatch.begin(Xylem::GlyphSortType::FRONT_TO_BACK);

    _level.draw(_spriteBatch, _camera);

    _player->draw(_spriteBatch, _camera);

    for (size_t i = 0; i < _bullets.size(); ++i) {
        _bullets[i]->draw(_spriteBatch, _camera);
    }

    for (size_t i = 0; i < _civilians.size(); ++i) {
        _civilians[i]->draw(_spriteBatch, _camera);
    }

    for (size_t i = 0; i < _zombies.size(); ++i) {
        _zombies[i]->draw(_spriteBatch, _camera);
    }

    _spriteBatch.end();

    _spriteBatch.renderBatch();

    _textureProgram.unuse();

    // Swap our buffer and draw everything to the screen!
    _window.swapBuffer();
}

void GameManager::constructLevel(unsigned int levelID)
{
    _player = new Player(glm::vec2(0.0f, 0.0f));

    _level.clean();

    _level.load(levelID);

    _player->giveGun(new Gun(1, 20.0f, 0.4f, 100.0f, 400, 1, 0.05f, &_bullets)); // Sniper
    _player->giveGun(new Gun(2, 5.0f, 0.2f, 20.0f, 8, 8, 1.0f, &_bullets)); // Shotgun
    _player->giveGun(new Gun(5, 5.0f, 0.05f, 10.0f, 12, 1, 0.1f, &_bullets)); // Handgun
    _player->giveGun(new Gun(2, 5.0f, 20.0f, 0.1f, 8, 12, 1.0f, &_bullets)); // Blowback

    for (size_t i = 0; i < 50; ++i) {
        _civilians.push_back(new Civilian(glm::normalize(glm::vec2(((float)i), (-1.0f * (float)i))), glm::vec2((-220.0f + 50.0f * (float)i), (-200.0f + 50.0f * (float)i))));
    }

    _zombies.push_back(new Zombie(glm::vec2(0.0f), glm::vec2(200.0f, -200.0f)));
}
