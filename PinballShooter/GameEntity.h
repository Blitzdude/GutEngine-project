#pragma once

enum class EntityType {
	ET_PLAYER,
	ET_ENEMY,
	ET_PINBALL,
	ET_SIZE
};

class GameEntity
{
public:
	GameEntity();
	~GameEntity();

	virtual EntityType getEntityType() = 0;

};

