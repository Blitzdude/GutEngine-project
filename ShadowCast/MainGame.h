#pragma once

#include <Gutengine/Camera2D.h>
#include <Gutengine/SpriteBatch.h>
#include <Gutengine/InputManager.h>
#include <Gutengine/Window.h>
#include <Gutengine/GLSLProgram.h>
#include <Gutengine/Timing.h>
#include <Gutengine/SpriteFont.h>
#include <memory>

#include "BallController.h"
#include "BallRenderer.h"
#include "Grid.h"

// TODO:
// Visualize momentum with color
// Visualize velocity with color
// Visualize position with color

enum class GameState { RUNNING, EXIT };

const int CELL_SIZE = 12;

class MainGame {
public:
    ~MainGame();
    void run();


private:
    void init();
    void initRenderers();
    void initBalls();
    void update(float deltaTime);
    void draw();
    void drawHud();
    void processInput();

    int m_screenWidth = 0;
    int m_screenHeight = 0;

    std::vector<Ball> m_balls; ///< All the balls
    std::unique_ptr<Grid> m_grid; ///< Grid for spatial partitioning for collision

    int m_currentRenderer = 0;
    std::vector<std::unique_ptr<BallRenderer> > m_ballRenderers;

    BallController m_ballController; ///< Controls balls

    Gutengine::Window m_window; ///< The main window
    Gutengine::SpriteBatch m_spriteBatch; ///< Renders all the balls
    std::unique_ptr<Gutengine::SpriteFont> m_spriteFont; ///< For font rendering
    Gutengine::Camera2D m_camera; ///< Renders the scene
    Gutengine::InputManager m_inputManager; ///< Handles input
    Gutengine::GLSLProgram m_textureProgram; ///< Shader for textures]

    Gutengine::FpsLimiter m_fpsLimiter; ///< Limits and calculates fps
    float m_fps = 0.0f;

    GameState m_gameState = GameState::RUNNING; ///< The state of the game
};

