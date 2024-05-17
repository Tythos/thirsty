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

    // add sprite one
    thirsty::Node* one = new thirsty::Node();
    newNode(one);
    app->scene->children.push_back(*one);

    // add sprite two
    thirsty::Node* two = new thirsty::Node();
    newNode(two);
    app->scene->children.push_back(*two);

    // add sprite three
    thirsty::Node* three = new thirsty::Node();
    newNode(three);
    app->scene->children.push_back(*three);
    
    // main loop
    std::cout << "Running..." << std::endl;
    app->is_running = true;
    while (app->is_running) {
        thirsty::loopEvent(app);
        thirsty::loopRender(app);
    }

    // cleanup
    delete one;
    delete two;
    delete three;
    std::cout << "Exiting..." << std::endl;
    thirsty::freeApplication(app);
    return 0;
}
