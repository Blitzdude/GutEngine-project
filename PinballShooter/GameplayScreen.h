#pragma once

#include <Gutengine/GutengineErrors.h>
#include <Gutengine\IGameScreen.h>
#include <Gutengine\SpriteBatch.h>
#include <Gutengine\GLSLProgram.h>
#include <Gutengine\Camera2D.h>
#include <Gutengine\GLTexture.h>
#include <Gutengine\Window.h>
#include <Gutengine/GUI.h>
#include <Gutengine\DebugRenderer.h>
#include <Gutengine/AudioEngine.h>
#include <Box2D\Box2D.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

#include <vector>
#include <map>

#include "Box.h"
#include "Player.h"
#include "Enemy.h"
#include "Pinball.h"
#include "RayCastCallback.h"
#include "ContactListener.h"

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
	
	Gutengine::AudioEngine m_audioEngine;
	Gutengine::Music m_audioTrack;
	Gutengine::SoundEffect m_fireEffect;
	Gutengine::GUI m_gui;

	bool m_renderDebug = true;

	Player m_player;
	std::vector<Enemy*> m_enemies;
	std::vector<Pinball*> m_pinballs;

	std::unique_ptr<b2World> m_world;
	std::vector<b2Body*> m_callbackResults;

	//std::vector<Box> m_boxes;
	std::vector<float> m_corners;
	RayCastCallback m_rcCallback;
	ContactListener m_contactListenerInstance;

};

