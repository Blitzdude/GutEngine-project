#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <Gutengine/Gutengine.h>
#include <Gutengine/GLSLProgram.h>
#include <Gutengine/GLTexture.h>
#include <Gutengine/Sprite.h>
#include <Gutengine/Window.h>
#include <Gutengine/InputManager.h>
#include <Gutengine/Timing.h>

#include <Gutengine/SpriteBatch.h>

#include <Gutengine/Camera2D.h>

#include <vector>

#include "Level.h"

enum class GameState {PLAY, EXIT};

//Our example game class, just for testing purposes right now
class MainGame
{
public:
    MainGame();
    ~MainGame();

    void run();

private:
    void initSystems();
    void initShaders();
    void gameLoop();
    void processInput();
    void drawGame();

    Gutengine::Window _window;
    int _screenWidth;
    int _screenHeight;
    GameState _gameState;

    Gutengine::GLSLProgram _colorProgram;
    Gutengine::Camera2D _camera;

    Gutengine::SpriteBatch _spriteBatch;

    Gutengine::InputManager _inputManager;
    Gutengine::FpsLimiter _fpsLimiter;

    float _maxFPS;
    float _fps;
    float _time;

	std::vector<Level*> _levels; //< vector of all levels
};

