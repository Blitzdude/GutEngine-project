#include "MainMenuScreen.h"

#include "ScreenIndices.h"

MainMenuScreen::MainMenuScreen(Gutengine::Window * window) : m_window(window)
{
	m_screenIndex = SCREEN_INDEX_MAINMENU;
}

MainMenuScreen::~MainMenuScreen()
{
}

int MainMenuScreen::getNextScreenIndex() const
{
	return SCREEN_INDEX_GAMEPLAY;
}

int MainMenuScreen::getPreviousScreenIndex() const
{
	return SCREEN_INDEX_NO_SCREEN;
}

void MainMenuScreen::build()
{
}

void MainMenuScreen::destroy()
{
}

void MainMenuScreen::onEntry()
{
	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	initUI();
}

void MainMenuScreen::onExit()
{
	m_gui.destroy();
}

void MainMenuScreen::update()
{
	m_camera.update();
	checkInput();
}

void MainMenuScreen::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); ///< Clear gl color and depth buffers
	glClearColor(0.0f, 0.25f, 0.0f, 1.0f); ///< set Clear color to Solid red

	m_gui.draw();
}

void MainMenuScreen::initUI()
{
	m_gui.init("GUI");
	m_gui.loadScheme("TaharezLook.scheme");
	m_gui.setFont("DejaVuSans-10");
	CEGUI::PushButton* exitButton = static_cast<CEGUI::PushButton*>(m_gui.createWidget("TaharezLook/Button", glm::vec4(0.1f, 0.1f, 0.1f, 0.05f), glm::vec4(0.0f), "ExitButton"));
	exitButton->setText("Exit Game");

	// set event function to be handled, when button is clicked
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::onExitClicked, this));

	CEGUI::Combobox* myComboBox = static_cast<CEGUI::Combobox*>(m_gui.createWidget("TaharezLook/Combobox", glm::vec4(0.2f, 0.2f, 0.1f, 0.05f), glm::vec4(0.0f), "myComboBox"));

	m_gui.setMouseCursor("TaharezLook/MouseArrow");
	m_gui.showMouseCursor();
	SDL_ShowCursor(0);
}

void MainMenuScreen::checkInput()
{
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_gui.onSDLEvent(evnt);
	}
}

bool MainMenuScreen::onExitClicked(const CEGUI::EventArgs & e)
{
	std::cout << "IM QUITTING!" << std::endl;
	m_currentState = Gutengine::ScreenState::EXIT_APPLICATION;

	return true;
}
