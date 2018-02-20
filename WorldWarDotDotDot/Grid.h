#pragma once

#include <glm\glm.hpp>

#include <vector>
#include <Gutengine/Vertex.h>
#include <Gutengine/SpriteBatch.h>

// Cell contains force vector?
struct  Cell
{
	Gutengine::ColorRGBA8 color = Gutengine::ColorRGBA8(255, 0, 255, 255);
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
	
	int getNumXCells() const { return m_numXCells; };
	int getNumYCells() const { return m_numYCells; };

	/*
	void draw(Gutengine::SpriteBatch& spriteBatch, const glm::vec2& pos) {
		glm::vec4 destRect;
		destRect.x = (float)pos.x - m_cellSize / 2.0f;
		destRect.y = (float)pos.y - m_cellSize / 2.0f;
		destRect.z = (float)m_cellSize;
		destRect.w = (float)m_cellSize;
		spriteBatch.draw(destRect, 0.0f, getCell(pos)->color);
	}
	*/

private:
	std::vector<Cell> m_cells;
	int m_cellSize;
	int m_width;
	int m_height;
	int m_numXCells;
	int m_numYCells;
};

