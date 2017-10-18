#include <SDL\SDL.h>
#include <Gutengine\GutengineErrors.h>

#include <iostream>

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);


	printf("success\n");
	int a;
	std::cin >> a;

	return 0;
}