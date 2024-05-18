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
    //app->scene->isVisible = false;

    // add sprite one
    thirsty::Node* one = new thirsty::Node();
    newNode(one);
    one->transform[3][0] = 1.0;
    app->scene->children.push_back(*one);

    // add sprite two
    thirsty::Node* two = new thirsty::Node();
    newNode(two);
    two->transform[3][1] = 1.0;
    app->scene->children.push_back(*two);

    // add sprite three
    thirsty::Node* three = new thirsty::Node();
    newNode(three);
    three->transform[3][0] = -1.0;
    three->transform[3][1] = -1.0;
    app->scene->children.push_back(*three);
    
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
    delete one;
    delete two;
    delete three;
    delete app;
    return 0;
}
