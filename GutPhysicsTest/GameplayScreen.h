#pragma once

#include <Gutengine/IGameScreen.h>
#include <vector>
#include <Gutengine/SpriteBatch.h>
#include <Gutengine/GLSLProgram.h>
#include <Gutengine/Camera2D.h>
#include <Gutengine/GLTexture.h>
#include <Gutengine/Window.h>
#include <Gutengine/DebugRenderer.h>
#include <Gutengine/GutPhysics2D.h>

#include <Gutengine/GUI.h>


// Our custom gameplay screen that inherits from the IGameScreen
class GameplayScreen : public Gutengine::IGameScreen {
public:
    GameplayScreen(Gutengine::Window* window);
    ~GameplayScreen();

    virtual int getNextScreenIndex() const override;

    virtual int getPreviousScreenIndex() const override;

    virtual void build()	override;

    virtual void destroy()	override;

    virtual void onEntry()	override;

    virtual void onExit()	override;

    virtual void update()	override;

    virtual void draw()		override;

private:
    void initUI();
	void updateUI();
    void checkInput();
	void handleWrapping();

    bool onExitClicked(const CEGUI::EventArgs& e);

    Gutengine::SpriteBatch	 m_spriteBatch;
    Gutengine::GLSLProgram	 m_textureProgram;
    Gutengine::GLSLProgram	 m_lightProgram;
    Gutengine::Camera2D		 m_camera;
    Gutengine::GLTexture	 m_texture;
    Gutengine::Window*		 m_window;
    Gutengine::DebugRenderer m_debugRenderer;
    Gutengine::GUI			 m_gui;
							 
	CEGUI::Window* fpsText = nullptr;
	glm::vec2 m_torquePoint;
	std::weak_ptr<Gutengine::RigidBody> m_selectedShape;
	std::unique_ptr<Gutengine::GutPhysics2D> m_physicsSystem = nullptr;
	
    bool m_renderDebug = true;
	
};

