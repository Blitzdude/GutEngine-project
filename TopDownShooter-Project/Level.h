#pragma once

#include <string>
#include <vector>

#include <Gutengine\SpriteBatch.h>


class Level
{
public:
	// Constructor loads level
	Level(const std::string& fileName);
	~Level();

	void draw();
private:
	std::vector<std::string> _levelData;
	int _numHumans;
	Gutengine::SpriteBatch _spriteBatch;

	// can have multiple players, but have only one for now
	std::vector<glm::ivec2> _playerStartPositions;
	std::vector<glm::ivec2> _zombieStartPositions;
};

