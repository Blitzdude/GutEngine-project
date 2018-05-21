#pragma once
#include <vector>

#include <Gutengine/IGameScreen.h>
#include <Box2D/Box2D.h>
#include <Gutengine/SpriteBatch.h>
#include <Gutengine/SpriteFont.h>
#include <Gutengine/GLSLProgram.h>
#include <Gutengine/GLTexture.h>
#include <Gutengine/Camera2D.h>
#include <Gutengine/GLTexture.h>
#include <Gutengine/Window.h>
#include <Gutengine/DebugRenderer.h>
#include <Gutengine/GUI.h>

#include "Grid.h"
#include "Object.h"
#include "Circle.h"

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
    void createBoundary();
    void checkInput();
	void updateMouse();
    void visualizeForce(glm::vec2 cellPos);
    void makeEdge(float x, float y, float w, float h);
    void drawDebug(glm::mat4 projMat);
    void drawUI();
       

	// CEGUI methods
    bool onExitClicked(const CEGUI::EventArgs& e);
    bool onTeamToggleClicked(const CEGUI::EventArgs& e);


	// attributes
    Gutengine::SpriteBatch		m_spriteBatch;
    Gutengine::SpriteFont       m_spriteFont;
    Gutengine::GLSLProgram		m_textureProgram;
    Gutengine::Camera2D			m_camera;
    Gutengine::Camera2D			m_uiCamera;

    Gutengine::Window*			m_window;
    Gutengine::DebugRenderer	m_debugRenderer;
    Gutengine::GUI				m_gui;
    Gutengine::GLTexture        m_background;
    std::vector<b2Body*>        m_boundaries;
	
    bool m_renderDebug = true;
	bool m_mouse1 = false;
    bool m_mouse2 = false;
	bool m_isForceDirty = false;
    Team m_currentTeam = Team::BLUE;

	std::vector<glm::vec2> m_mouseCoordVector;
	std::vector<Circle> m_objects;

	std::unique_ptr<Grid> m_grid;
    std::unique_ptr<b2World> m_world;
};

