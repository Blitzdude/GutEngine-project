#include "Level.h"

#include <iostream>
#include <fstream>
#include <Gutengine\GutengineErrors.h>
#include <GutEngine\ResourceManager.h>



Level::Level(const std::string & fileName)

{
	

	std::fstream file;
	file.open(fileName); //open file

	//error checking
	if (file.fail()) {
		Gutengine::fatalError("Failed to open " + fileName);
	}

	//throw away the first string in temp
	std::string tmp;
	file >> tmp >> _numHumans;

	while (std::getline(file, tmp)) {
		_levelData.push_back(tmp);
	}

	// TEST: print out level data to console
	/*
	for (int i = 0; i < _levelData.size(); i++) {
		std::cout << _levelData[i] << std::endl;
	}
	*/

	_spriteBatch.begin();
	for (int y = 0; y < _levelData.size(); y++) // y-column
	{
		for (int x = 0; x < _levelData[y].size(); x++)
		{
			// grab the tile
			char tile = _levelData[y][x];

			// process the tile
			switch (tile) {
			case 'R':


			}
		}
	}
	_spriteBatch.end();
}


Level::~Level()
{
}

void Level::draw()
{
}
