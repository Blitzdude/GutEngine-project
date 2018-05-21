#include "Grid.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <SDL/SDL.h>


Grid::Grid(int width, int height, int cellSize) : 
	m_width(width),
	m_height(height),
	m_cellSize(cellSize)
{
	m_numXCells = (int)ceil((float)m_width / m_cellSize);
	m_numYCells = (int)ceil((float)m_height / m_cellSize);

	m_cells.resize(m_numYCells * m_numXCells);
    std::cout << "loading cells" << std::endl;

    for (int j = 0; j < m_numYCells; j++)
    {
        for (int i = 0; i < m_numXCells; i++)
        {
            auto c = getCell(glm::vec2(i, j));
            assert(c != nullptr);
            c->setGridPos(i, j);
        }
    }
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

    auto forceNotZero = [&](int x, int y) {return getCell(x, y)->getForce() != glm::vec2(0.0f, 0.0f); };

	// 8-directional
	// Top-Left
    if (forceNotZero(CenterX - 1, CenterY + 1))
	    result.push_back(getCellPos(CenterX - 1, CenterY + 1));
	// Top
    if (forceNotZero(CenterX, CenterY + 1))
	    result.push_back(getCellPos(CenterX, CenterY + 1));
	// Top-Right
    if (forceNotZero(CenterX + 1, CenterY + 1))
	    result.push_back(getCellPos(CenterX + 1, CenterY + 1));
	// Right
    if (forceNotZero(CenterX + 1, CenterY ))
	    result.push_back(getCellPos(CenterX + 1, CenterY));
	// Down-Right
    if (forceNotZero(CenterX + 1, CenterY - 1))
	    result.push_back(getCellPos(CenterX + 1, CenterY - 1));
	// Down
    if (forceNotZero(CenterX, CenterY - 1))
	    result.push_back(getCellPos(CenterX, CenterY - 1));
	// Down-Left
    if (forceNotZero(CenterX - 1, CenterY - 1))
	    result.push_back(getCellPos(CenterX - 1, CenterY - 1));
	// Left
    if (forceNotZero(CenterX - 1, CenterY))
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
    std::vector<glm::vec2> initialL;
    std::vector<glm::vec2> oldL;
    std::vector<glm::vec2> newL;

    // set first the the values in oldList
    for (auto i = input.begin(); std::next(i) != input.end(); ++i)
    {
        auto c = getCell(*i);
        if (c->isSet == false)
        {
            c->setForce(glm::normalize(*std::next(i) - *i));
            c->isSet = true;
            initialL.push_back(*i);
        }
    }

    oldL = initialL;
    // repeat this n times:
    while (n > 0)
    {
        for (auto old_itr : oldL) // for each element in old L.
        {
            // get neighbors.
            std::vector<glm::vec2> neighbors = getCellNeighbors4Directions(old_itr);

            // push them to new list
            for (auto n_itr = neighbors.begin(); n_itr != neighbors.end(); n_itr++)
            {
                // but only if it's not in the old list or the new list

                if (std::find(oldL.begin(), oldL.end(), *n_itr) == oldL.end())
                {
                    // new
                    newL.push_back(*n_itr);
                }

            }
        }
        glm::vec2 sum(0.0f);
        // now for each neighbor 
        for (auto m_itr : newL)
        {

            //get its surrounding cells
            auto surCells = getCellNeighbors8Directions(m_itr);
            int numNeighbors = surCells.size();
            for (auto s_itr = surCells.begin(); s_itr != surCells.end(); s_itr++)
            {
                // sum the force values
                sum += getCell(*s_itr)->getForce();
            }
            // add bias = normalized vector from neighbor to closest point in the sketched list
            sum += glm::normalize(findPointWithLowestDistance(initialL, m_itr) - m_itr) * 2.0f;
            // divide by number of neighbors as per flocking.
            sum /= numNeighbors;

            auto cell = getCell(m_itr);
            if (!cell->isSet )
                cell->setForce(glm::normalize(sum));

        }
        // now add the elements in new list to old list
        for (auto itr : newL)
            oldL.push_back(itr);

        for (auto itr : oldL)
            getCell(itr)->isSet = false;
        // then clear the new list to be reused. 
        newL.clear();
        n--;
    }

}

glm::vec2 Grid::findPointWithLowestDistance(std::vector<glm::vec2> list, glm::vec2 pos)
{

    glm::vec2 lowest = list.back();
    for (auto itr : list)
    {
        float d = glm::dot(itr - pos, itr - pos);
        float l = glm::dot(lowest - pos, lowest - pos);

        if (d < l)
            lowest = itr;
    }
    return lowest;
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

void Grid::resetForces()
{
    for (int j = 0; j < m_numYCells; ++j)
        for (int i = 0; i < m_numXCells; ++i) {
            auto c = getCell(i, j);
            c->setForce({ 0.0f, 0.0f });
            c->isSet = false;
        }
}
