/**
 * thirsty.hpp
 */

#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
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
    };

    struct Renderer {
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

    // void freeApplication(Application* app);

    void newShaders(Application* app);

    void newGeometries(Application* app);

    void newMaterials(Application* app);

    // void renderLoop(Application* app);
}
