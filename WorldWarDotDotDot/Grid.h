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
struct  Cell
{
	Gutengine::ColorRGBA8 color;// = Gutengine::ColorRGBA8(255, 255, 255, 255);
	glm::vec2 force {0.0f, 0.0f};

	void setForce(const glm::vec2& vec)
	{
		force.x = vec.x;
		force.y = vec.y;
		color = Gutengine::ColorRGBA8(force.x * 255, force.y * 255, 0, 255);
	}
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

private:
	
	std::vector<Cell> m_cells;
	int m_cellSize;
	int m_width;
	int m_height;
	int m_numXCells;
	int m_numYCells;
	bool m_isDirty;
};

