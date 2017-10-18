#pragma once

#include <Gutengine/Window.h>
#include <Gutengine/GLSLProgram.h>
#include <Gutengine/Camera2D.h>
#include <Gutengine/InputManager.h>
#include <Gutengine/SpriteBatch.h>
#include <Gutengine/SpriteFont.h>
#include <Gutengine/AudioEngine.h>
#include <Gutengine/ParticleEngine2D.h>
#include <Gutengine/ParticleBatch2D.h>

#include "Player.h"
#include "Level.h"
#include "Bullet.h"

class Zombie;

enum class GameState {
    PLAY,
    EXIT
};

class MainGame
{
public:
    MainGame();
    ~MainGame();

    /// Runs the game
    void run();

private:
    /// Initializes the core systems
    void initSystems();

    /// Initializes the level and sets up everything
    void initLevel();

    /// Initializes the shaders
    void initShaders();

    /// Main game loop for the program
    void gameLoop();

    /// Updates all agents
    void updateAgents(float deltaTime);

    /// Updates all bullets
    void updateBullets(float deltaTim);

    /// Checks the victory condition
    void checkVictory();

    /// Handles input processing
    void processInput();

    /// Renders the game
    void drawGame();

    /// Draws the HUD
    //void drawHud();

    /// Adds blood to the particle engine
    void addBlood(const glm::vec2& position, int numParticles);

    /// Member Variables
    Gutengine::Window m_window; ///< The game window
    
    Gutengine::GLSLProgram m_textureProgram; ///< The shader program

    Gutengine::InputManager m_inputManager; ///< Handles input

    Gutengine::Camera2D m_camera; ///< Main Camera
    Gutengine::Camera2D m_hudCamera; ///< Hud Camera

    Gutengine::SpriteBatch m_agentSpriteBatch; ///< Draws all agents
    Gutengine::SpriteBatch m_hudSpriteBatch;

    Gutengine::ParticleEngine2D m_particleEngine;
    Gutengine::ParticleBatch2D* m_bloodParticleBatch;

    std::vector<Level*> m_levels; ///< vector of all levels

    int m_screenWidth = 1024;
    int m_screenHeight = 768;

    float m_fps;

    int m_currentLevel;

    Player* m_player;
    std::vector<Human*> m_humans; ///< Vector of all humans
    std::vector<Zombie*> m_zombies; ///< Vector of all zombies
    std::vector<Bullet> m_bullets;

    int m_numHumansKilled; ///< Humans killed by player
    int m_numZombiesKilled; ///< Zombies killed by player

    //Gutengine::SpriteFont* m_spriteFont;

    Gutengine::AudioEngine m_audioEngine;

    GameState m_gameState;
};

