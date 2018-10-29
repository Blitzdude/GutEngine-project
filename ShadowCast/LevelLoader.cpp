#include "LevelLoader.h"

#include <Gutengine\IOManager.h>



Level::Level()
{
}


Level::~Level()
{
}

LevelData Level::LoadLevel(const std::string & p_FilePath)
{
	// Load file to buffer
	std::string l_buffer;
	Gutengine::IOManager::readFileToBuffer(p_FilePath, l_buffer);

	LevelData data;
	// Parse buffer data
		// Level size (x,y)
		// num enemies
		// Each cell, check type (nothing, tile, window, etc...)
		// If tile -> check neighbors for edges
		// Edge pool stuff

	// check each character for cell
	// make 
	return LevelData();
}
