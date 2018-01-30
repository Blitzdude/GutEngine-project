#pragma once

#include "Human.h"
#include <Gutengine/InputManager.h>
#include <Gutengine/Camera2D.h>
#include "Bullet.h"

class Gun;

class Player : public Human
{
public:
    Player();
    ~Player();

    void init(float speed, glm::vec2 pos, Gutengine::InputManager* inputManager, Gutengine::Camera2D* camera, std::vector<Bullet>* bullets);

    void addGun(Gun* gun);

    void update(const std::vector<std::string>& levelData,
                std::vector<Human*>& humans,
                std::vector<Zombie*>& zombies,
                float deltaTime) override;
private:
    Gutengine::InputManager* _inputManager;

    std::vector<Gun*> _guns;
    int _currentGunIndex;

    Gutengine::Camera2D* _camera;
    std::vector<Bullet>* _bullets;

};

