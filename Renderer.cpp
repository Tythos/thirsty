/**
 * Renderer.cpp
 */

#include "thirsty.hpp"

void thirsty::newRenderer(thirsty::Renderer* renderer) {
    // create window
    renderer->window = SDL_CreateWindow("Application Rendering Context", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    if (renderer->window == NULL) {
        std::cerr << "Creating main window failed!" << std::endl;
        SDL_Quit();
        throw std::exception();
    }

    // initialize GL context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    renderer->context = SDL_GL_CreateContext(renderer->window);
    if (renderer->context == NULL) {
        std::cerr << "Creating GL context failed!" << std::endl;
        SDL_DestroyWindow(renderer->window);
        SDL_Quit();
        throw std::exception();
    }

    // initialize glew
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Initializing GLEW failed!" << std::endl;
        SDL_GL_DeleteContext(renderer->context);
        SDL_DestroyWindow(renderer->window);
        SDL_Quit();
        throw std::exception();
    }
}

void thirsty::freeRenderer(thirsty::Renderer* renderer) {
    SDL_GL_DeleteContext(renderer->context);
    SDL_DestroyWindow(renderer->window);
}

void thirsty::render(thirsty::Renderer* renderer, thirsty::Node* scene, thirsty::Node* camera) {
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    SDL_GL_SwapWindow(renderer->window);
}
