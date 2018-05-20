#pragma once

#include <glm/glm.hpp>

// Gutengine
#include <Gutengine/Vertex.h>
#include <Gutengine/SpriteBatch.h>
#include <Gutengine/GLTexture.h>
#include <Gutengine/InputManager.h>
#include <Gutengine/Camera2D.h>

// standard library
#include <vector>

// Cell contains force vector?
class Cell
{
public:
    
	void setForce(const glm::vec2& vec)
	{
		force.x = vec.x;
		force.y = vec.y;
	}

    glm::vec2 getForce() const { return force; };

    void setColor(const Gutengine::ColorRGBA8 c) { color = c; };
    void setColor(int r, int g, int b, int a) { setColor(Gutengine::ColorRGBA8(r, g, b, a)); };

    Gutengine::ColorRGBA8& const getColor() { return color; };

    void setGridPos(int x, int y) 
    {
        gridPosX = x;
        gridPosY = y;
    }

    glm::vec2 getGridPos() { return glm::vec2(gridPosX, gridPosY); };
    bool isSet = false;
private:
	Gutengine::ColorRGBA8 color;
    glm::vec2 force = { 0.0f, 0.0f };
    int gridPosX;
    int gridPosY;
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

	//
	std::vector<glm::vec2> getCellNeighbors4Directions(glm::vec2 pos);
	std::vector<glm::vec2> getCellNeighbors8Directions(glm::vec2 pos);
	
	void createFlowField(std::vector<glm::vec2> list, int n = 3);
	glm::vec2 sumForceAverage(std::vector<glm::vec2> list);

	
	// Getters
	glm::vec2 getCellPos(int x, int y) { return glm::vec2(x * m_cellSize, y * m_cellSize);  };
	glm::vec2 getCellPos(const glm::vec2& pos) { return glm::vec2(int(pos.x) * m_cellSize, int(pos.y) * m_cellSize); };
	
	int getNumXCells() const { return m_numXCells; };
	int getNumYCells() const { return m_numYCells; };
	
	Gutengine::GLTexture m_cellTexture;

	void update(Gutengine::InputManager &inputManager, Gutengine::Camera2D &camera);
    void resetForces();
   
private:
	
	std::vector<Cell> m_cells;
	int m_cellSize;
	int m_width;
	int m_height;
	int m_numXCells;
	int m_numYCells;
};

