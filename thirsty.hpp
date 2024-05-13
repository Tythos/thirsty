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
    struct Application;
    struct Renderer;
    struct Scene;
    struct Camera;
    struct SpriteNode;
    struct Transform;
    struct Geometry;
    struct Material;
    struct Texture;
    struct Shader;
    
    struct Application {
        Renderer* renderer = NULL;
        Scene* scene = NULL;
        Camera* camera = NULL;
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

    struct Renderer {
        SDL_Window* window = NULL;
        SDL_GLContext context = 0;
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
    void newRenderer(Renderer* renderer);
    void newShaders(Application* app);
    void newGeometries(Application* app);
    void newMaterials(Application* app);

    void freeMaterials(Application* app);
    void freeRenderer(Renderer* renderer);
    void freeGeometries(Application* app);
    void freeShaders(Application* app);
    void freeApplication(Application* app);

    void loopRender(Application* app);
    void loopEvent(Application* app);
    void render(Renderer* renderer, Scene* scene, Camera* camera);

    const char* getSource(const char* path);
}
