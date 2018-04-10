#pragma once

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

#include "Grid.h"
#include "Object.h"

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
	// methods
    void initUI();
    void checkInput();

	// CEGUI methods
    bool onExitClicked(const CEGUI::EventArgs& e);

	// attributes
    Gutengine::SpriteBatch		m_spriteBatch;
    Gutengine::GLSLProgram		m_textureProgram;
    // Gutengine::GLSLProgram	m_lightProgram;
    Gutengine::Camera2D			m_camera;
    Gutengine::GLTexture		m_texture;
    Gutengine::Window*			m_window;
    Gutengine::DebugRenderer	m_debugRenderer;
    Gutengine::GUI				m_gui;
	
    bool m_renderDebug = true;
	bool m_mouse1 = false;
	bool m_isForceDirty = false;

	std::vector<glm::vec2> m_mouseCoordVector;
	std::vector<Object> m_objects;

	std::unique_ptr<Grid> m_grid;
    std::unique_ptr<b2World> m_world;
};

