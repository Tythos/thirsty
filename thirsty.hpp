/**
 * thirsty.hpp
 */

#pragma once

#include <fstream>
#include <iostream>
#include <deque>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <stack>
#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <SDL_image.h>

namespace thirsty {
    struct Application;
    struct Camera;
    struct Geometry;
    struct Material;
    struct Node;
    struct Renderer;
    
    struct Application {
        Renderer* renderer = NULL;
        Node* scene = NULL;
        Camera* camera = NULL;
        bool is_running;
    };

    struct Camera {
        float fov_rad = (float)(0.2 * M_PI);
        float aspectRatio = (float)1.0;
        float nearPlane = (float)0.1;
        float farPlane = (float)10.0;
        glm::vec3 xyz = { 0.0f, 0.0f, 10.0f };
        glm::vec3 look = { 0.0f, 0.0f, -1.0f };
        glm::vec3 up = { 0.0f, 1.0f, 0.0f };
    };

    struct Geometry {
        std::vector<GLfloat> verts = {
            // xyuv format; recall img y is flipped
            -1.0, -1.0, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 0.0f
        };
        std::vector<GLint> indices = {
            0, 1, 2, 0, 2, 3
        };
        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint ebo = 0;
    };

    struct Material {
        SDL_Surface* texture = NULL;
        GLuint tex = 0;
        GLuint vert_shader = 0;
        GLuint frag_shader = 0;
        GLuint shader_prog = 0;
    };

    struct Node {
        glm::mat4x4 transform = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        Geometry* geometry = NULL;
        Material* material = NULL;
        std::vector<Node> children;
        bool isVisible = true;
    };

    struct Renderer {
        SDL_Window* window = NULL;
        SDL_GLContext context = 0;
        const int width = 800;
        const int height = 600;
    };

    void newApplication(Application* app);
    void newCamera(Camera* camera);
    void newGeometry(Geometry* geometry);
    void newMaterial(Material* material);
    void newNode(Node* node);
    void newRenderer(Renderer* renderer);

    void freeApplication(Application* app);
    void freeCamera(Camera* camera);
    void freeGeometry(Geometry* geometry);
    void freeMaterial(Material* material);
    void freeNode(Node* node);
    void freeRenderer(Renderer* renderer);

    glm::mat4 getViewMatrix(Camera* camera);
    glm::mat4 getProjectionMatrix(Camera* camera);
    void loopRender(Application* app);
    void loopEvent(Application* app);
    void render(Renderer* renderer, Node* scene, Camera* camera);
    void renderNode(thirsty::Renderer* renderer, thirsty::Node* node, std::stack<glm::mat4>& matrixStack, const glm::mat4& view, const glm::mat4& projection);
    const char* getSource(const char* path);
}
