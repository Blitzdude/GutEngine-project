#pragma once
#include <Gutengine/IGameScreen.h>
#include <Gutengine/Window.h>
#include <Gutengine/Camera2D.h>

#include <Gutengine/GUI.h>
#include <CEGUI/CEGUI.h>


class MainMenuScreen : public Gutengine::IGameScreen {
public:
	MainMenuScreen(Gutengine::Window* window); //< constructor
	~MainMenuScreen();


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

	void initUI();
	void checkInput();


	bool onExitClicked(const CEGUI::EventArgs& e);

	Gutengine::Camera2D m_camera; //< camera window
	Gutengine::Window* m_window;

	Gutengine::GUI m_gui;

};