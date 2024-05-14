/**
 * Application.cpp
 */

#include "thirsty.hpp"

void thirsty::newApplication(Application* app) {
    // we manage SDL initialization at the application level so we can subsequently organize threading, mixing, etc. as well as video
    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Initializing SDL video failed!" << std::endl;
        throw std::exception();
    }

    // remaining initializations are now managed by renderer
    app->renderer = new thirsty::Renderer();
    thirsty::newRenderer(app->renderer);

    // we also populate a single top-level node
    app->scene = new thirsty::Node();
    newNode(app->scene);
}

void thirsty::loopRender(Application* app) {
    thirsty::render(app->renderer, app->scene, app->camera);
}

void thirsty::loopEvent(Application* app) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
            app->is_running = false;
            break;
        }
    }
}

void thirsty::freeApplication(Application* app) {
    thirsty::freeNode(app->scene);
    thirsty::freeRenderer(app->renderer);
    SDL_Quit();
}

const char* thirsty::getSource(const char* path) {
    // reads contents of file and returns the allocated character buffer
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << path << std::endl;
        return nullptr;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    char* charBuffer = new char[content.size() + 1];
    std::copy(content.begin(), content.end(), charBuffer);
    charBuffer[content.size()] = '\0';
    return charBuffer;
}
