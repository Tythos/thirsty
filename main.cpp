/**
 * main.cpp
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_image.h>

#include "thirsty.hpp"

SDL_Surface* logo_rgba;

const GLfloat verts[6][4] = {
    { -1.0f, -1.0f, 0.0f, 1.0f },
    { -1.0f, 1.0f, 0.0f, 0.0f },
    { 1.0f, 1.0f, 1.0f, 0.0f },
    { 1.0f, -1.0f, 1.0f, 1.0f }
};

const GLint indicies[] = {
    0, 1, 2, 0, 2, 3
};

void freeApplication(thirsty::Application* app) {
    glUseProgram(0);
    glDisableVertexAttribArray(0);
    glDetachShader(app->shader_prog, app->vert_shader);
    glDetachShader(app->shader_prog, app->frag_shader);
    glDeleteProgram(app->shader_prog);
    glDeleteShader(app->vert_shader);
    glDeleteShader(app->frag_shader);
    glDeleteTextures(1, &app->tex);
    glDeleteBuffers(1, &app->ebo);
    glDeleteBuffers(1, &app->vbo);
    glDeleteVertexArrays(1, &app->vao);

    // invoke delete/destroy methods for SDL state
    SDL_GL_DeleteContext(app->context);
    SDL_DestroyWindow(app->window);
    SDL_Quit();
}

const char* getSource(const char* path) {
    // reads contents of file and returns the allocated character buffer
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << path << std::endl;
        return nullptr;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    char* charBuffer = new char[content.size() + 1];
    std::copy(content.begin(), content.end(), charBuffer);
    charBuffer[content.size()] = '\0';
    return charBuffer;
}

void initShaders(thirsty::Application* app) {
    GLint status;
    char err_buf[512];
    glGenVertexArrays(1, &(app->vao));
    glBindVertexArray(app->vao);

    // compile vertex shader
    app->vert_shader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexSource = getSource("basic.v.glsl");
    glShaderSource(app->vert_shader, 1, &vertexSource, NULL);
    glCompileShader(app->vert_shader);
    glGetShaderiv(app->vert_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog(app->vert_shader, sizeof(err_buf), NULL, err_buf);
        err_buf[sizeof(err_buf)-1] = '\0';
        std::cerr << "Compiling vertex shader failed!" << std::endl;
        std::cerr << err_buf << std::endl;
        return;
    }
    delete[] vertexSource;

    // compile fragment shader
    app->frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentSource = getSource("basic.f.glsl");
    glShaderSource(app->frag_shader, 1, &fragmentSource, NULL);
    glCompileShader(app->frag_shader);
    glGetShaderiv(app->frag_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog(app->frag_shader, sizeof(err_buf), NULL, err_buf);
        err_buf[sizeof(err_buf)-1] = '\0';
        std::cerr << "Compiling fragment shader failed!" << std::endl;
        std::cerr << err_buf << std::endl;
        return;
    }
    delete[] fragmentSource;

    // link shader program
    app->shader_prog = glCreateProgram();
    glAttachShader(app->shader_prog, app->vert_shader);
    glAttachShader(app->shader_prog, app->frag_shader);
    glBindFragDataLocation(app->shader_prog, 0, "oRGBA");
    glLinkProgram(app->shader_prog);
    glUseProgram(app->shader_prog);
    return;
}

void initGeometries(thirsty::Application* app) {
    // Populate vertex and element buffers
    glGenBuffers(1, &app->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, app->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glGenBuffers(1, &app->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    // bind vertex position and texture coordinate attributes
    GLint pos_attr_loc = glGetAttribLocation(app->shader_prog, "aXY");
    glVertexAttribPointer(pos_attr_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0); // 2 values; stride 4, offset by 0
    glEnableVertexAttribArray(pos_attr_loc);
    GLint tex_attr_loc = glGetAttribLocation(app->shader_prog, "aUV");
    glVertexAttribPointer(tex_attr_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat))); // 2 values; stride 4, offset by 2
    glEnableVertexAttribArray(tex_attr_loc);
}

void initMaterials(thirsty::Application* app) {
    // results in the successful transcription of raw image bytes into a uniform texture buffer in the GPU program
    glGenTextures(1, &app->tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, app->tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glUniform1i(glGetUniformLocation(app->shader_prog, "uTexture"), 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // define texture sampling parameters and map raw image data
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, logo_rgba->pixels);
}

void renderLoop(thirsty::Application* app) {
    // to render: clear color buffer; draw elements (constant program references); and swap buffers
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    SDL_GL_SwapWindow(app->window);
}

int main(int nArgs, char** vArgs) {
    // startup
    std::cout << "Initializing..." << std::endl;
    std::string filename = "logo.png";
    logo_rgba = IMG_Load(filename.c_str());
    thirsty::Application* app = new thirsty::Application();
    thirsty::newApplication(app);
    initShaders(app);
    initGeometries(app);
    initMaterials(app);

    // main loop
    std::cout << "Running..." << std::endl;
    bool is_running = true;
    while (is_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
                is_running = false;
                break;
            }
        }
        renderLoop(app);
    }

    // cleanup
    std::cout << "Exiting..." << std::endl;
    freeApplication(app);
    SDL_FreeSurface(logo_rgba);
    logo_rgba = NULL;
    return 0;
}
