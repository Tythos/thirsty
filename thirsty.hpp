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
#include <SDL_image.h>

namespace thirsty {
    struct Application {
        SDL_Window* window = NULL;
        SDL_GLContext context = 0;
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
        bool is_running = false;
    };

    struct Scene {
    };

    struct Camera {
    };

    struct SpriteNode {
    };

    struct Transform {
    };

    struct Geometry {
    };

    struct Material {
    };

    struct Texture {
    };
    
    struct Shader {
    };

    void newApplication(Application* app);
    void newShaders(Application* app);
    void newGeometries(Application* app);
    void newMaterials(Application* app);

    void freeMaterials(Application* app);
    void freeGeometries(Application* app);
    void freeShaders(Application* app);
    void freeApplication(Application* app);

    void loopRender(Application* app);
    void loopEvent(Application* app);

    const char* getSource(const char* path);
}
