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
    struct Renderer;
    struct Node;
    struct Transform;
    struct Geometry;
    struct Material;
    struct Texture;
    struct Shader;
    
    struct Application {
        Renderer* renderer = NULL;
        Node* scene = NULL;
        Node* camera = NULL;
        bool is_running;
    };

    struct Renderer {
        SDL_Window* window = NULL;
        SDL_GLContext context = 0;
    };

    struct Node {
        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint ebo = 0;
        GLuint tex = 0;
        GLuint vert_shader = 0;
        GLuint frag_shader = 0;
        GLuint shader_prog = 0;
        std::vector<GLfloat> verts;
        std::vector<GLint> indices;
        SDL_Surface* logo_rgba;
    };

    struct Transform {
        glm::mat4 M;
    };

    struct Geometry {
        // vertex format
        // vertex buffer
        // index buffer
    };

    struct Material {
        // texture, shader, and uniform hooks
    };

    struct Texture {
        // SDL surface
    };
    
    struct Shader {
        // encapsulation of vertex & fragment shaders, compiled into a specific program
    };

    void newApplication(Application* app);
    void newGeometries(Node* app);
    void newMaterials(Node* app);
    void newNode(Node* node);
    void newRenderer(Renderer* renderer);
    void newShaders(Node* app);

    void freeApplication(Application* app);
    void freeGeometries(Node* app);
    void freeMaterials(Node* app);
    void freeNode(Node* node);
    void freeRenderer(Renderer* renderer);
    void freeShaders(Node* app);

    void loopRender(Application* app);
    void loopEvent(Application* app);
    void render(Renderer* renderer, Node* scene, Node* camera);
    const char* getSource(const char* path);
}
