/**
 * main.cpp
 */

#include <iostream>
#include "thirsty.hpp"

int main(int nArgs, char** vArgs) {
    // startup
    std::cout << "Initializing..." << std::endl;
    thirsty::Application* app = new thirsty::Application();
    thirsty::newApplication(app);

    // main loop
    std::cout << "Running..." << std::endl;
    app->is_running = true;
    while (app->is_running) {
        thirsty::loopEvent(app);
        thirsty::loopRender(app);
    }

    // cleanup
    std::cout << "Exiting..." << std::endl;
    thirsty::freeApplication(app);
    return 0;
}
