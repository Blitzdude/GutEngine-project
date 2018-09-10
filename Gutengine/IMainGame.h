#pragma once

#include "Gutengine.h"
#include "Window.h"
#include "InputManager.h"
#include <memory>

namespace Gutengine 
{

class ScreenList;
class IGameScreen;

class IMainGame 
{
public:
    IMainGame();
    virtual ~IMainGame();

    // Runs and initializes the game
    void run();
    // Exits the game
    void exitGame();

    // Called on initialization
    virtual void onInit() = 0;
    // For adding all screens
    virtual void addScreens() = 0;
    // Called when exiting
    virtual void onExit() = 0;

    void onSDLEvent(SDL_Event& evnt);

    const float getFps() const 
    {
		return m_fps;
    }

	const float getDeltaTime() const
	{
		return m_deltaTime == 0.0f ? 1.0f : m_deltaTime;
	}
    InputManager inputManager;

protected:
    // Custom update function
    virtual void update();
    // Custom render function
    virtual void draw();

    bool init();
	bool init(const char* name, int width, int height);

	bool initSystems();
    bool initSystems(const char* name, int width, int height);


    std::unique_ptr<ScreenList> m_screenList = nullptr;
    IGameScreen* m_currentScreen			 = nullptr;
    bool m_isRunning						 = false;
    float m_fps								 = 0.0f;
	float m_deltaTime = 0.0f;
    Window m_window;
};

} // namespace end