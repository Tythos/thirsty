/**
 * main.cpp
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_image.h>
#include "thirsty.hpp"

int main(int nArgs, char** vArgs) {
    // startup
    std::cout << "Initializing..." << std::endl;
    thirsty::Application* app = new thirsty::Application();
    thirsty::newApplication(app);
    thirsty::newShaders(app);
    thirsty::newGeometries(app);
    thirsty::newMaterials(app);

    // main loop
    std::cout << "Running..." << std::endl;
    app->is_running = true;
    while (app->is_running) {
        thirsty::loopEvent(app);
        thirsty::loopRender(app);
    }

    // cleanup
    std::cout << "Exiting..." << std::endl;
    thirsty::freeMaterials(app);
    thirsty::freeGeometries(app);
    thirsty::freeShaders(app);
    thirsty::freeApplication(app);
    return 0;
}
