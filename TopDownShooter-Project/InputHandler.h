#pragma once
#include <Gutengine/InputManager.h>
#include <SDL/SDL_keycode.h>
#include "Commands.h"

class InputHandler
{
public:
    InputHandler(Gutengine::InputManager& p_inputManager);
    ~InputHandler();

    void handleInput();

    // Methods to bind commands
    void bindButtons();

private:
    Gutengine::InputManager* m_inputManager;
    Command* m_fireButton;
    Command* m_moveUpButton;
    Command* m_moveDownButton;
    Command* m_moveLeftButton;
    Command* m_moveRightButton;
};

InputHandler::InputHandler(Gutengine::InputManager& p_inputManager)
    : m_inputManager(&p_inputManager)
{
    bindButtons();
}

InputHandler::~InputHandler()
{
    delete m_fireButton; m_fireButton = nullptr;
    delete m_moveDownButton; m_moveDownButton = nullptr;
    delete m_moveUpButton; m_moveUpButton = nullptr;
    delete m_moveRightButton; m_moveRightButton = nullptr;
    delete m_moveLeftButton; m_moveLeftButton = nullptr;
}

void InputHandler::handleInput()
{
    if (m_inputManager->isKeyPressed(SDLK_SPACE)) m_fireButton->execute();
    else if (m_inputManager->isKeyPressed(SDLK_w)) m_moveUpButton->execute();
    else if (m_inputManager->isKeyPressed(SDLK_s)) m_moveDownButton->execute();
    else if (m_inputManager->isKeyPressed(SDLK_a)) m_moveLeftButton->execute();
    else if (m_inputManager->isKeyPressed(SDLK_d)) m_moveRightButton->execute();
}

void InputHandler::bindButtons()
{
    m_fireButton = new FireCommand();
    m_moveUpButton = new MoveUpCommand();
    m_moveDownButton = new MoveDownCommand();
    m_moveRightButton = new MoveRightCommand();
    m_moveLeftButton = new MoveLeftCommand();
}

