#pragma once

#include <glm\glm.hpp>

#include <vector>
#include <Gutengine/Vertex.h>
#include <Gutengine/SpriteBatch.h>
#include <Gutengine/GLTexture.h>
#include <Gutengine/InputManager.h>

// Cell contains force vector?
struct  Cell
{
	Gutengine::ColorRGBA8 color = Gutengine::ColorRGBA8(255, 255, 255, 255);
	//glm::vec2 pos;
};

class Grid
{
public:
	Grid(int width, int height, int cellSize);
	~Grid();

	// Getters
	// get cell based on cell coordinates
	Cell* getCell(int x, int y);
	// get cell based on window coordinates
	Cell* getCell(const glm::vec2& pos);

	glm::vec2 getCellPos(int x, int y) { return glm::vec2(x * m_cellSize, y * m_cellSize);  };
	glm::vec2 getCellPos(const glm::vec2& pos) { return glm::vec2(pos.x * m_cellSize, pos.y * m_cellSize); };

	int getNumXCells() const { return m_numXCells; };
	int getNumYCells() const { return m_numYCells; };
	
	Gutengine::GLTexture m_cellTexture;

	void update(Gutengine::InputManager &inputManager);

private:
	std::vector<Cell> m_cells;
	int m_cellSize;
	int m_width;
	int m_height;
	int m_numXCells;
	int m_numYCells;
};

