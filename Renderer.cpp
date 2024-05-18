/**
 * Renderer.cpp
 */

#include "thirsty.hpp"

void thirsty::newRenderer(thirsty::Renderer* renderer) {
    // create window
    renderer->window = SDL_CreateWindow("Application Rendering Context", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, renderer->width, renderer->height, SDL_WINDOW_OPENGL);
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

void thirsty::renderNode(thirsty::Renderer* renderer, thirsty::Node* node, std::stack<glm::mat4>& matrixStack, const glm::mat4& view, const glm::mat4& projection) {
    // update transform stack
    glm::mat4 currentTransform = matrixStack.top();
    glm::mat4 nodeTransform = currentTransform * node->transform;
    matrixStack.push(nodeTransform);

    if (node->isVisible && node->material != NULL && node->geometry != NULL) {
        // et shader program
        glUseProgram(node->material->shader_prog);

        // set uniform matrix locations
        GLint hModel = glGetUniformLocation(node->material->shader_prog, "uModel");
        GLint hView = glGetUniformLocation(node->material->shader_prog, "uView");
        GLint hProjection = glGetUniformLocation(node->material->shader_prog, "uProjection");
        glUniformMatrix4fv(hModel, 1, GL_FALSE, glm::value_ptr(nodeTransform));
        glUniformMatrix4fv(hView, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(hProjection, 1, GL_FALSE, glm::value_ptr(projection));

        // bind the texture
        if (node->material->texture) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, node->material->tex);
            GLint texLoc = glGetUniformLocation(node->material->shader_prog, "uTexture");
            glUniform1i(texLoc, 0);
        }

        // bind the vertices, then draw
        glBindVertexArray(node->geometry->vao);
        glDrawElements(GL_TRIANGLES, (GLsizei)node->geometry->indices.size(), GL_UNSIGNED_INT, 0);

        // unbind vertex array and texture; reset program
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
    }

    // recurse to children, then pop transform
    for (auto child : node->children) {
        renderNode(renderer, &child, matrixStack, view, projection);
    }
    matrixStack.pop();
}

void thirsty::render(thirsty::Renderer* renderer, thirsty::Node* scene, thirsty::Camera* camera) {
    // initialize stacks
    std::stack<glm::mat4>* transformStack = new std::stack<glm::mat4>();
    transformStack->push(glm::mat4(1.0f));
    
    // clear buffers
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDepthFunc(GL_LESS);

    // set view and projection matrics
    camera->aspectRatio = (float)(renderer->width) / (float)(renderer->height);
    glm::mat4 view = thirsty::getViewMatrix(camera);
    glm::mat4 projection = thirsty::getProjectionMatrix(camera);

    // begin traversal
    renderNode(renderer, scene, *transformStack, view, projection);
    SDL_GL_SwapWindow(renderer->window);
}
