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
    // initialize stacks
    std::stack<glm::mat4>* transformStack = new std::stack<glm::mat4>();
    transformStack->push(glm::mat4(1.0f));
    std::deque<Node*> toTraverse = {scene};
    
    // clear buffers
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // set view and projection matrics
    float fov_rad = 0.5 * M_PI;
    float aspectRatio = 1.0;
    float nearPlane = 0.1;
    float farPlane = 10.0;
    glm::vec3 cameraPos = { 0.0f, 0.0f, 1.0f };
    glm::vec3 cameraFront = { 0.0f, 0.0f, -1.0f };
    glm::vec3 cameraUp = { 0.0f, 1.0f, 0.0f };
    glm::mat4 view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(fov_rad), aspectRatio, nearPlane, farPlane);

    // start traversal
    while (toTraverse.size() > 0) {
        Node* currentNode = toTraverse.front();
        toTraverse.pop_front();
        transformStack->push(currentNode->transform);

        // set shader uniforms
        GLint hModel = glGetUniformLocation(currentNode->material->shader_prog, "model");
        GLint hView = glGetUniformLocation(currentNode->material->shader_prog, "view");
        GLint hProjection = glGetUniformLocation(currentNode->material->shader_prog, "projection");
        glUniformMatrix4fv(hModel, 1, GL_FALSE, &transformStack->top()[0][0]);
        glUniformMatrix4fv(hView, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(hProjection, 1, GL_FALSE, &projection[0][0]);

        // bind node geometry
        glBindVertexArray(currentNode->geometry->vao);

        // load shader program
        glUseProgram(currentNode->material->shader_prog);

        // draw elements
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

        // add children to traversal queue
        for (auto child : currentNode->children) {
            toTraverse.push_back(&child);
        }
    }

    // once finished, swap
    SDL_GL_SwapWindow(renderer->window);
}
