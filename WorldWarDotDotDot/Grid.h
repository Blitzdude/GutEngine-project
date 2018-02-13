#pragma once

#include <vector>
#include <Gutengine/Vertex.h>

// Cell contains force vector?
struct  Cell
{
	Gutengine::ColorRGBA8 color;
};

class Grid
{
public:
	Grid(int width, int height, int cellSize);
	~Grid();

	// Getters
	Cell* getCell(int x, int y);
	int getNumXCells() const { return m_numXCells; };
	int getNumYCells() const { return m_numYCells; };


private:
	std::vector<Cell> m_cells;
	int m_cellSize;
	int m_width;
	int m_height;
	int m_numXCells;
	int m_numYCells;
};

