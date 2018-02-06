#pragma once

#include "Player.h"
#include <Gutengine/IGameScreen.h>
#include <vector>
#include <Gutengine/SpriteBatch.h>
#include <Gutengine/GLSLProgram.h>
#include <Gutengine/Camera2D.h>
#include <Gutengine/GLTexture.h>
#include <Gutengine/Window.h>
#include <Gutengine/DebugRenderer.h>

#include <Gutengine/GUI.h>

#include "ScreenIndices.h"

class MainMenuScreen : public Gutengine::IGameScreen {
public:
    MainMenuScreen(Gutengine::Window* window);
    ~MainMenuScreen();

    virtual int getNextScreenIndex() const override;

    virtual int getPreviousScreenIndex() const override;

    virtual void build() override;

    virtual void destroy() override;

    virtual void onEntry() override;

    virtual void onExit() override;

    virtual void update() override;

    virtual void draw() override;

private:
    void initUI();
    void checkInput();

    bool onNewGameClicked(const CEGUI::EventArgs& e);
    bool onEditorClicked(const CEGUI::EventArgs& e);
    bool onExitClicked(const CEGUI::EventArgs& e);

    int m_nextScreenIndex = SCREEN_INDEX_GAMEPLAY;
    Gutengine::Camera2D m_camera;
    Gutengine::Window* m_window;
    Gutengine::GUI m_gui;
};