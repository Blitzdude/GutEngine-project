#include "Grid.h"
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

std::vector<glm::vec2> Grid::getCellNeighbors8Directions(glm::vec2 pos)
{
	std::vector<glm::vec2> result;
	// center cell

	// 8-directional
	for (int j = -1; j < 2; ++j) {
		for (int i = -1; i < 2; ++i) {
			glm::vec2 neighbor = getCellPos((int)pos.x + i, (int)pos.y + j);
			// check that neighbor is not the same as the evaluated cell.
			if (neighbor != pos && getCell(neighbor)->force != glm::vec2(0.0f, 0.0f)) {
				result.push_back(neighbor);
			}
		}
	}
	
	return result;
}

std::vector<glm::vec2> Grid::getCellNeighbors4Directions(glm::vec2 pos)
{
	
	std::vector<glm::vec2> result;
	// center cell
	// get cells around the center cell

	// 4-directional
	// Top
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

	return result;
}

glm::vec2 Grid::sumForces(std::vector<glm::vec2> neighbors)
{
	glm::vec2 sumForce;
	// sum all of neighbors force elements. 
	for (auto& itr : neighbors)
		sumForce += getCell(itr)->force;
	// divide the sum by the number of elements. 
	sumForce = sumForce / (float)neighbors.size();
	return sumForce;
}

void Grid::createDirectionField(std::vector<glm::vec2> list, int n)
{
	//m_forcedCells.clear();
	//std::vector<glm::vec2> neighbors;

	for (auto& itr : list) {
		//m_forcedCells.push_back(getCell(itr));

		auto first = itr;
		auto second = std::next(&itr);
		getCell(itr)->setForce(glm::normalize(*second - first) * FORCE_MAX);
	
		/*
		auto evaluate = getCellNeighbors4Directions(itr);
		// calculate force for each cell
		for (auto& e_itr : evaluate)
		{
			auto forceVec = getCellNeighbors8Directions(e_itr);
			getCell(e_itr)->setForce(sumForces(neighbors));
			neighbors.push_back(e_itr);
		}
		*/
	}
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
