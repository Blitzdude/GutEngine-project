#include "MainGame.h"
#include <iostream>

int main(int argc, char** argv) {

	for (int i = 0; i < argc; i++) {
		std::cout << i << " " << argv[i] << std::endl;
	}

    MainGame mainGame;
    mainGame.run();

    return 0;
}