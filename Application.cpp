/**
 * Application.cpp
 */

#include "thirsty.hpp"

void thirsty::newApplication(Application* app) {
    // initialize SDL w/ video
    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Initializing SDL video failed!" << std::endl;
        throw std::exception();
    }

    // create window
    app->window = SDL_CreateWindow("Application", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    if (app->window == NULL) {
        std::cerr << "Creating main window failed!" << std::endl;
        SDL_Quit();
        throw std::exception();
    }

    // initialize GL context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    app->context = SDL_GL_CreateContext(app->window);
    if (app->context == NULL) {
        std::cerr << "Creating GL context failed!" << std::endl;
        SDL_DestroyWindow(app->window);
        SDL_Quit();
        throw std::exception();
    }

    // initialize glew
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Initializing GLEW failed!" << std::endl;
        SDL_GL_DeleteContext(app->context);
        SDL_DestroyWindow(app->window);
        SDL_Quit();
        throw std::exception();
    }
}

// void thirsty::freeApplication(Application* app) {}

void thirsty::newShaders(Application* app) {}

void thirsty::newGeometries(Application* app) {}

void thirsty::newMaterials(Application* app) {}

// void thirsty::renderLoop(Application* app) {}
