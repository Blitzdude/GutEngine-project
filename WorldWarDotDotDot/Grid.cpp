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
	// center cell
	// get cells around the center cell

	// 4-directional
	// Top
	result.push_back(glm::vec2(pos.x, pos.y + 1));

	// Right
	result.push_back(glm::vec2(pos.x + 1, pos.y));
	

	// Down
	result.push_back(glm::vec2(pos.x, pos.y - 1));
	
	// Left
	result.push_back(glm::vec2(pos.x - 1, pos.y));


	// 4-directional
	// Top
	/*
	if (glm::vec2(pos.x, pos.y + 1) != pos &&
		getCell(glm::vec2(pos.x, pos.y + 1))->force != glm::vec2(0.0f, 0.0f)) 
	{
		result.push_back(glm::vec2(pos.x, pos.y + 1));
	}
	// Right
	if (glm::vec2(pos.x + 1, pos.y) != pos &&
		getCell(glm::vec2(pos.x + 1, pos.y))->force != glm::vec2(0.0f, 0.0f))
	{
		result.push_back(glm::vec2(pos.x + 1, pos.y));
	}
	
	// Down
	if (glm::vec2(pos.x, pos.y - 1) != pos &&
		getCell(glm::vec2(pos.x, pos.y - 1))->force != glm::vec2(0.0f, 0.0f))
	{
		result.push_back(glm::vec2(pos.x, pos.y - 1));
	}
	// Left
	if (glm::vec2(pos.x - 1, pos.y) != pos &&
		getCell(glm::vec2(pos.x - 1, pos.y))->force != glm::vec2(0.0f, 0.0f))
	{
		result.push_back(glm::vec2(pos.x - 1, pos.y));
	}
	*/

	return result;
}


void Grid::createFlowField(std::vector<glm::vec2> input, int n)
{
	std::vector<glm::vec2> openList; // nodes to be evaluated

	for (auto itr : input) // push each element from the input list to openList
	{ 
		// check that same cell is not inserted twice
		getCell(itr)->color = Gutengine::ColorRGBA8(0, 0, 255, 255);
		openList.push_back(itr);
	}

	for (auto itr = openList.begin(); std::next(itr) != openList.end(); ++itr) // for all elements in openList get its cell and add a force 
 
	{
		// force = normalized(next - current) * 40
		getCell(*itr)->setForce(glm::normalize(*(std::next(itr)) - *itr) * 40.0f);
	}

	
	std::vector<glm::vec2> nextList;
	while (n > 0) 
	{
		// add neighbors to a new list to be iterated through
		for (auto itr : openList) 
		{
			auto neighbors = getCellNeighbors4Directions(itr);
			for (auto n_itr : neighbors) // neighbors
			{
				// check neighbor is not already in next list or previous list
				if (std::find(nextList.begin(), nextList.end(), n_itr) == nextList.end() &&
					std::find(openList.begin(), openList.end(), n_itr) == openList.end())
				{
					getCell(n_itr)->color = Gutengine::ColorRGBA8(255, 0, 0, 255);
					nextList.push_back(n_itr);
				}
				
			}
		}
		// for each element in the new list, sum the surrounding forces and set the cells force to that
		for (auto itr : nextList)
		{
			glm::vec2 sum = sumForceAverage(getCellNeighbors4Directions(itr));
			getCell(itr)->setForce(sum);
		}
		// add next lists elements to openList
		for (auto itr : nextList)
		{
			openList.push_back(itr);
		}
		// clear the nextList
		nextList.clear();
		n--; // repeat until n is 0
	}
	m_isDirty = true;
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
	if (m_isDirty)
	{
		for (int y = 0; y < getNumYCells(); y++)
		{
			for (int x = 0; x < getNumXCells(); x++)
			{
				//Cell *current = getCell(x, y);
				//current->color = Gutengine::ColorRGBA8(current->force.x, 0, current->force.y, 255);
			}
		}
	}
}
