#pragma once

#include <Gutengine/IMainGame.h>

#include "GameplayScreen.h"

#include "MainMenuScreen.h"

class App : public Gutengine::IMainGame {
public:
	App();
	~App();

	// implement virtual functions from IMainGame
	// called on initialization
	virtual void onInit() override;
	// for adding all screens
	virtual void addScreens() override;
	// called when exiting
	virtual void onExit() override;
private:
	std::unique_ptr<GameplayScreen> m_gameplayScreen = nullptr;
	std::unique_ptr<MainMenuScreen> m_mainMenuScreen = nullptr;
};

