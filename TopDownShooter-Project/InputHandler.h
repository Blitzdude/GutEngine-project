#pragma once
#include <Gutengine/InputManager.h>
#include <glm\glm.hpp>
#include <SDL/SDL_keycode.h>
#include "Commands.h"

class Agent;

class InputHandler
{
public:
    InputHandler(Gutengine::InputManager& p_inputManager, Agent& agent);
    ~InputHandler();

    Command* handleInput();

    // Methods to bind commands
    void bindButtons();

    glm::vec2 getMouseCoordinates() {return m_mouseCoords;};
private:
    Agent* m_agent;

    Gutengine::InputManager* m_inputManager;
    Command* m_fireButton;
    Command* m_moveUpButton;
    Command* m_moveDownButton;
    Command* m_moveLeftButton;
    Command* m_moveRightButton;

    glm::vec2 m_mouseCoords;
};

InputHandler::InputHandler(Gutengine::InputManager& p_inputManager, Agent& agent)
    : m_inputManager(&p_inputManager)
    , m_agent(&agent)
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

Command* InputHandler::handleInput()
{
    if (m_inputManager->isKeyPressed(SDLK_SPACE)) return m_fireButton;
    else if (m_inputManager->isKeyPressed(SDLK_w)) return m_moveUpButton;
    else if (m_inputManager->isKeyPressed(SDLK_s)) return m_moveDownButton;
    else if (m_inputManager->isKeyPressed(SDLK_a)) return m_moveLeftButton;
    else if (m_inputManager->isKeyPressed(SDLK_d)) return m_moveRightButton;

    // update mouse coordinates, not converted to world space
    m_mouseCoords = m_inputManager->getMouseCoords();
}

void InputHandler::bindButtons()
{
    m_fireButton      = new FireCommand();
    m_moveUpButton    = new MoveUpCommand();
    m_moveDownButton  = new MoveDownCommand();
    m_moveLeftButton  = new MoveLeftCommand();
    m_moveRightButton = new MoveRightCommand();
}

