#pragma once

class Command
{
public:
    virtual ~Command() {};
    virtual void execute() = 0;

private:

};

class MoveUpCommand : public Command
{
    virtual void execute() { moveUp();} override;
};

class MoveDownCommand : public Command
{
    virtual void execute() { moveDown(); } override;
};

class MoveLeftCommand : public Command
{
    virtual void execute() { moveLeft(); } override;
};

class MoveRightCommand : public Command
{
    virtual void execute() { moveRight(); } override;
};

class FireCommand : public Command
{
    virtual void execute() { fire(); } override;
};