#pragma once
#include <glm\common.hpp>

class Agent;

class Command
{
public:
    virtual ~Command() {};
    virtual void execute(Agent& p_agent) = 0;

private:

};

class MoveUpCommand : public Command
{
    virtual void execute(Agent& p_agent) override { p_agent.moveUp();};
};

class MoveDownCommand : public Command
{
    virtual void execute(Agent& p_agent) override { p_agent.moveDown(); };
};

class MoveLeftCommand : public Command
{
    virtual void execute(Agent& p_agent) override { p_agent.moveLeft(); };
};

class MoveRightCommand : public Command
{
    virtual void execute(Agent& p_agent) override { p_agent.moveRight(); };
};

class FireCommand : public Command
{
    virtual void execute(Agent& p_agent) override { p_agent.fire(); };
};