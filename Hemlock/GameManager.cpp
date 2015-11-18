#include "GameManager.h"

#include "Gun.h"

#include <Xylem\Xylem.h>
#include <Xylem\ResourceManager.h>

#include <SDL\SDL.h>

#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>

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

    const float DESIRED_FPS = 60.0f;
    const int MAX_PHYSICS_STEPS = 6;

    const float MS_PER_SECOND = 1000.0f;
    const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
    const float MAX_DELTA_TIME = 1.0f;

    float previousTicks = SDL_GetTicks();

    static int frameCount = 0;
    while (_gameState != GameState::EXIT && _gameState != GameState::EXIT_CONFIRMATION) {
        _fpsLimiter.begin();

        float newTicks = SDL_GetTicks();
        float frameTime = newTicks - previousTicks;
        previousTicks = newTicks;
        float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

        if (_zombies.size() == 0) {
            //constructLevel(++_levelID);
        }

        _inputManager.update();
        processInput();

        int i = 0;
        while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
            float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

            updatePlayer(deltaTime);

            updateBullets(deltaTime);

            updateCivilians(deltaTime);

            updateZombies(deltaTime);

            totalDeltaTime -= deltaTime;
            ++i;
        }

        _camera.update();

        drawGame();

        _fps = _fpsLimiter.end();
        if (++frameCount % 600 == 0) {
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

void GameManager::updatePlayer(float deltaTime)
{
    _player->update(deltaTime);

    _camera.setPosition(_player->getPosition());

    _level.collideEntity(*_player);
}

void GameManager::updateBullets(float deltaTime)
{
    for (size_t i = 0; i < _bullets.size();) {
        // Update bullets.
        if (_bullets[i]->update(deltaTime) == false) {
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
}

void GameManager::updateCivilians(float deltaTime)
{
    for (size_t i = 0; i < _civilians.size();) {
        // Collide with player.
        _civilians[i]->collideWithEntity(*_player);
        // Collide with civilians.
        for (size_t j = i + 1; j < _civilians.size(); ++j) {
            _civilians[i]->collideWithEntity(*(_civilians[j]));
        }
        // Update civilian location.
        if (_civilians[i]->update(deltaTime) == false) {
            _civilians[i] = _civilians.back();
            _civilians.pop_back();
        } else {
            ++i;
        }
    }
}

void GameManager::updateZombies(float deltaTime)
{
    for (size_t i = 0; i < _zombies.size();) {
        // Collide with civilians.
        for (size_t j = 0; j < _civilians.size();) {
            if (_zombies[i]->collideWithEntity(*(_civilians[j]))) {
                glm::vec2 civilianLocation = _civilians[j]->getPosition();
                _civilians[j] = _civilians.back();
                _civilians.pop_back();
                _zombies.push_back(new Zombie(glm::vec2(0.0f, 0.0f), civilianLocation, _player, &_civilians));
            } else {
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
        if (_zombies[i]->update(deltaTime) == false) {
            _zombies[i] = _zombies.back();
            _zombies.pop_back();
        } else {
            ++i;
        }
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
    _level.clean();

    _level.load(levelID);

    _player = new Player(_level.getPlayerStartingLocation(), &_inputManager, &_camera);
    
    for (auto gunDatum : _level.getGunData()) {
        _player->giveGun(new Gun(gunDatum[0], gunDatum[1], gunDatum[2], gunDatum[3], gunDatum[4], gunDatum[5], gunDatum[6], &_bullets));
    }

    std::mt19937 randomEngine;
    randomEngine.seed(time(nullptr));

    std::uniform_int_distribution<int> randX(2, _level.getMapWidth() - 2);
    std::uniform_int_distribution<int> randY(2, _level.getMapHeight() - 2);

    for (size_t i = 0; i < _level.getStartingHumanCount(); ++i) {
        _civilians.
    }

    for (auto zombieLocation : _level.getZombieStartPositions()) {
        _zombies.emplace_back(glm::vec2(0.0f), zombieLocation, &_player, &_civilians);
    }
    /*for (size_t i = 0; i < 50; ++i) {
        _civilians.push_back(new Civilian(glm::normalize(glm::vec2(((float)i), (-1.0f * (float)i))), glm::vec2((-220.0f + 50.0f * (float)i), (-200.0f + 50.0f * (float)i))));
    }

    _zombies.push_back(new Zombie(glm::vec2(0.0f), glm::vec2(200.0f, -200.0f)), _player, _civilians);*/
}
