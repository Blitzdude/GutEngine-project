#include <Gutengine/IMainGame.h>

/*
	31.7.2018
	-Started importing code from SAT-project
	-Created RigidBody base class (different from Rigidbody2D for now)
	-NEXT: finish changing Rectangle, and Particle to inherit from RigidBody Base class;
*/
 

/*
	rectangles: xy - bottomLeft
	width, height: - full
*/
#include "App.h"

int main(int argc, char** argv) {
    App app;
    app.run();
    return 0;
}
