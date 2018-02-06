#pragma once

#include "Box.h"
#include "Player.h"
#include <Gutengine/IGameScreen.h>
#include <Box2D/Box2D.h>
#include <vector>
#include <Gutengine/SpriteBatch.h>
#include <Gutengine/GLSLProgram.h>
#include <Gutengine/Camera2D.h>
#include <Gutengine/GLTexture.h>
#include <Gutengine/Window.h>
#include <Gutengine/DebugRenderer.h>

#include <Gutengine/GUI.h>

// Our custom gameplay screen that inherits from the IGameScreen
class GameplayScreen : public Gutengine::IGameScreen {
public:
    GameplayScreen(Gutengine::Window* window);
    ~GameplayScreen();

    virtual int getNextScreenIndex() const override;

    virtual int getPreviousScreenIndex() const override;

    virtual void build() override;

    virtual void destroy() override;

    virtual void onEntry() override;

    virtual void onExit() override;

    virtual void update() override;

    virtual void draw() override;

private:
    void initUI();
    void checkInput();

    bool onExitClicked(const CEGUI::EventArgs& e);

    Gutengine::SpriteBatch m_spriteBatch;
    Gutengine::GLSLProgram m_textureProgram;
    Gutengine::GLSLProgram m_lightProgram;
    Gutengine::Camera2D m_camera;
    Gutengine::GLTexture m_texture;
    Gutengine::Window* m_window;
    Gutengine::DebugRenderer m_debugRenderer;
    Gutengine::GUI m_gui;

    bool m_renderDebug = false;

    Player m_player;
    std::vector<Box> m_boxes;
    std::unique_ptr<b2World> m_world;
};

