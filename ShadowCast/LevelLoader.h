#pragma once


#include <string>
#include <glm\glm.hpp>
struct Edge
{
	glm::vec2 start;
	glm::vec2 end;
};

struct Cell
{
	int edgeId[4];
	bool EdgeExists[4];
	bool Exists = false;
};

struct LevelData
{
	int levelWidth;
	int levelHeight;

};

class Level
{
public:
	Level();
	~Level();

	LevelData LoadLevel(const std::string& p_FilePath);
};

