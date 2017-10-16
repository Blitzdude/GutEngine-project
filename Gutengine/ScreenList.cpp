#include "ScreenList.h"

#include "IGameScreen.h"

namespace Gutengine {

    ScreenList::ScreenList(IMainGame* game) :
        m_game(game) {
        // Empty
    }

    ScreenList::~ScreenList() {
        destroy();
    }

    IGameScreen* Gutengine::ScreenList::moveNext() {
        IGameScreen* currentScreen = getCurrent();
        if (currentScreen->getNextScreenIndex() != SCREEN_INDEX_NO_SCREEN) {
            m_currentScreenIndex = currentScreen->getNextScreenIndex();
        }
        return getCurrent();
    }

    IGameScreen* Gutengine::ScreenList::movePrevious() {
        IGameScreen* currentScreen = getCurrent();
        if (currentScreen->getPreviousScreenIndex() != SCREEN_INDEX_NO_SCREEN) {
            m_currentScreenIndex = currentScreen->getPreviousScreenIndex();
        }
        return getCurrent();
    }

    void Gutengine::ScreenList::setScreen(int nextScreen) {
        m_currentScreenIndex = nextScreen;
    }

    void Gutengine::ScreenList::addScreen(IGameScreen* newScreen) {
        newScreen->m_screenIndex = m_screens.size();
        m_screens.push_back(newScreen);
        newScreen->build();
        newScreen->setParentGame(m_game);
    }

    void Gutengine::ScreenList::destroy() {
        for (size_t i = 0; i < m_screens.size(); i++) {
            m_screens[i]->destroy();
        }
        m_screens.resize(0);
        m_currentScreenIndex = SCREEN_INDEX_NO_SCREEN;
    }

    IGameScreen* ScreenList::getCurrent() {
        if (m_currentScreenIndex == SCREEN_INDEX_NO_SCREEN) return nullptr;
        return m_screens[m_currentScreenIndex];
    }

}