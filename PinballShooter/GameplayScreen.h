#pragma once

// Gutengine includes
#include <Gutengine\IGameScreen.h>
#include <Gutengine\SpriteBatch.h>
#include <Gutengine\GLSLProgram.h>
#include <Gutengine\Camera2D.h>
#include <Gutengine\GLTexture.h>
#include <Gutengine\Window.h>
#include <Gutengine\DebugRenderer.h>
// Third party
#include <Box2D\Box2D.h>
// STL
#include <vector>
#include <map>

#include "Player.h"

class GameplayScreen : public Gutengine::IGameScreen {
public:
	GameplayScreen(Gutengine::Window* window); //< constructor
	~GameplayScreen();


	// Return the index of the next or previous screen when changing screens
	virtual int getNextScreenIndex() const override;

	virtual int getPreviousScreenIndex() const override;

	// Called at beginning and end of application
	virtual void build() override;

	virtual void destroy() override;

	// Called when a screen enters and exits focus
	virtual void onEntry() override;

	virtual void onExit() override;

	// Called in the main game loop
	virtual void update() override;

	virtual void draw() override;

private:
	void checkInput();

	void initShaders();

	void makeLevelEdges();
	void makeEdge(float x, float y , float w, float h);


	Gutengine::SpriteBatch m_spriteBatch;
	Gutengine::GLSLProgram m_textureProgram; //< renders textures
	Gutengine::Camera2D m_camera; //< camera window
	Gutengine::GLTexture m_texture;
	Gutengine::Window* m_window;
	Gutengine::DebugRenderer m_debugRenderer;

	bool m_renderDebug = true;


	std::unique_ptr<b2World> m_world;
	Player m_player;
};

