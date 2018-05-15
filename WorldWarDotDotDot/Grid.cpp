#include "Grid.h"
#include <vector>
#include <algorithm>
#include <SDL/SDL.h>

const float FORCE_MAX = 24.0f;

Grid::Grid(int width, int height, int cellSize) : 
	m_width(width),
	m_height(height),
	m_cellSize(cellSize)
{
	m_numXCells = (int)ceil((float)m_width / m_cellSize);
	m_numYCells = (int)ceil((float)m_height / m_cellSize);

	m_cells.resize(m_numYCells * m_numXCells);
}

Grid::~Grid()
{
}

Cell* Grid::getCell(int x, int y)
{
	// bound x and y positions within the cell grid
	if (x < 0)
		x = 0;
	if (x >= m_numXCells)
		x = m_numXCells - 1;

	if (y < 0)
		y = 0;
	if (y >= m_numYCells)
		y = m_numYCells - 1;
	// ---

	return &m_cells[y * m_numXCells + x];
}

Cell* Grid::getCell(const glm::vec2 & pos)
{
	// return integer, so mouse position corresponds to cell
	int cellX = (int)(pos.x / m_cellSize);
	int cellY = (int)(pos.y / m_cellSize);

	return getCell(cellX, cellY);
}

std::vector<glm::vec2> Grid::getCellNeighbors4Directions(glm::vec2 pos)
{
	std::vector<glm::vec2> result;
	// get cells around the center cell
	int CenterX = (int)(pos.x / m_cellSize);
	int CenterY = (int)(pos.y / m_cellSize);

	// 4-directional
	// Top
	result.push_back(getCellPos(CenterX, CenterY + 1));

	// Down
	result.push_back(getCellPos(CenterX, CenterY - 1));

	// Right
	result.push_back(getCellPos(CenterX + 1, CenterY));

	// Left
	result.push_back(getCellPos(CenterX - 1, CenterY));

	return result;
}

std::vector<glm::vec2> Grid::getCellNeighbors8Directions(glm::vec2 pos)
{
	std::vector<glm::vec2> result;
	// get cells around the center cell
	int CenterX = (int)(pos.x / m_cellSize);
	int CenterY = (int)(pos.y / m_cellSize);

	// 8-directional
	// Top-Left
	result.push_back(getCellPos(CenterX - 1, CenterY + 1));
	// Top
	result.push_back(getCellPos(CenterX, CenterY + 1));
	// Top-Right
	result.push_back(getCellPos(CenterX + 1, CenterY + 1));
	// Right
	result.push_back(getCellPos(CenterX + 1, CenterY));
	// Down-Right
	result.push_back(getCellPos(CenterX + 1, CenterY - 1));
	// Down
	result.push_back(getCellPos(CenterX, CenterY - 1));
	// Down-Left
	result.push_back(getCellPos(CenterX - 1, CenterY - 1));
	// Left
	result.push_back(getCellPos(CenterX - 1, CenterY));
	
	return result;
}

// Input is mouseCoordVector
void Grid::createFlowField(std::vector<glm::vec2> input, int n /* = 3*/)
{
	/************//*
	1. add forces to initial cells

	loop n times
	2. get neighbors of initial cells
		-> check neighbor not in initial list
	3. add neighbors to nextList
	4. sumAverage the forces for neighbors
	5. add cells in next list to initial list
	6. clear nextlist
				  
	*/



	std::list<glm::vec2> initialList;
	std::list<glm::vec2> nextList;

	for (auto itr = input.begin(); std::next(itr) != input.end(); itr++)
	{
		if (std::find(initialList.begin(), initialList.end(), getCellPos(*itr)) == initialList.end()) // check that same cell is not inserted twice
		{
			auto cell = getCell(*itr);
			cell->setForce(glm::normalize(*std::next(itr) - *itr));
			glm::vec2 cPos = getCellPos(*itr);
			initialList.push_back(cPos);
		}
	}
	// we now have InitialList with cells containing forces. 

	for (auto itr = initialList.begin(); itr != initialList.end(); ++itr)
	{
		std::vector<glm::vec2> neighbors = getCellNeighbors4Directions(*itr);
		for (auto n_itr = neighbors.begin(); n_itr != neighbors.end(); ++n_itr)
		{
			// check neighbor is not in initialList and not in the nextList
			if (std::find(initialList.begin(), initialList.end(), getCellPos(*n_itr)) == initialList.end() &&
				std::find(nextList.begin(), nextList.end(), getCellPos(*n_itr)) == nextList.end())
			{
			
				nextList.push_back(*n_itr);
			}
		}
	}

	// now go through the next list and average sum vectors around it. 
	for (auto itr : nextList)
	{
		glm::vec2 sum = { 0.0f, 0.0f };
		for (auto itr_n : getCellNeighbors8Directions(itr))
		{
			sum += itr_n;
		}

		getCell(itr)->setForce(glm::normalize(sum));
	}
	

	


}

glm::vec2 Grid::sumForceAverage(std::vector<glm::vec2> list)
{
	glm::vec2 sum = { 0.0f, 0.0f };
	for (auto itr : list)
		sum += itr;

	// divide by size of list
	sum / (float)list.size();
	return sum;
}



void Grid::update(Gutengine::InputManager &inputManager, Gutengine::Camera2D &camera)
{
	// Camera is needed to convert mouse coordinates

	// if mouse 1 is down
	if (inputManager.isKeyDown(SDL_BUTTON_LEFT)) {
		// get the cell pointed to by mouse
		Cell* cell = getCell(camera.convertScreenToWorld(inputManager.getMouseCoords()));

	}
	
}
