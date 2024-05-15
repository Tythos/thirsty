/**
 * thirsty.hpp
 */

#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SDL_image.h>

namespace thirsty {
    struct Application;
    struct Geometry;
    struct Material;
    struct Node;
    struct Renderer;
    struct Transform;
    
    struct Application {
        Renderer* renderer = NULL;
        Node* scene = NULL;
        Node* camera = NULL;
        bool is_running;
    };

    struct Geometry {
        std::vector<GLfloat> verts;
        std::vector<GLint> indices;
        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint ebo = 0;
    };

    struct Material {
        SDL_Surface* texture;
        GLuint tex = 0;
        GLuint vert_shader = 0;
        GLuint frag_shader = 0;
        GLuint shader_prog = 0;
    };

    struct Node {
        Geometry* geometry = NULL;
        Material* material = NULL;
    };

    struct Renderer {
        SDL_Window* window = NULL;
        SDL_GLContext context = 0;
    };

    struct Transform {
        glm::mat4 M;
    };

    void newApplication(Application* app);
    void newGeometry(Geometry* geometry);
    void newMaterial(Material* material);
    void newNode(Node* node);
    void newRenderer(Renderer* renderer);

    void freeApplication(Application* app);
    void freeGeometry(Geometry* geometry);
    void freeMaterial(Material* material);
    void freeNode(Node* node);
    void freeRenderer(Renderer* renderer);

    void loopRender(Application* app);
    void loopEvent(Application* app);
    void render(Renderer* renderer, Node* scene, Node* camera);
    const char* getSource(const char* path);
}
