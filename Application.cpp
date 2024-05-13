/**
 * Application.cpp
 */

#include "thirsty.hpp"

void thirsty::newApplication(Application* app) {
    // we manage SDL initialization at the application level so we can subsequently organize threading, mixing, etc. as well as video
    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Initializing SDL video failed!" << std::endl;
        throw std::exception();
    }

    // remaining initializations are now managed by renderer
    app->renderer = new thirsty::Renderer();
    thirsty::newRenderer(app->renderer);
}

void thirsty::freeMaterials(Application* app) {
    glDeleteTextures(1, &app->tex);
    SDL_FreeSurface(app->logo_rgba);
    app->logo_rgba = NULL;
}

void thirsty::freeGeometries(Application* app) {
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &app->ebo);
    glDeleteBuffers(1, &app->vbo);
    glDeleteVertexArrays(1, &app->vao);
}

void thirsty::freeShaders(Application* app) {
    glUseProgram(0);
    glDetachShader(app->shader_prog, app->vert_shader);
    glDetachShader(app->shader_prog, app->frag_shader);
    glDeleteProgram(app->shader_prog);
    glDeleteShader(app->vert_shader);
    glDeleteShader(app->frag_shader);
}

void thirsty::freeApplication(Application* app) {
    thirsty::freeRenderer(app->renderer);
    SDL_Quit();
}

void thirsty::newShaders(Application* app) {
    GLint status;
    char err_buf[512];
    glGenVertexArrays(1, &(app->vao));
    glBindVertexArray(app->vao);

    // compile vertex sahder
    app->vert_shader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexSource = thirsty::getSource("basic.v.glsl");
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
    GLint isLinked = 0;
    glGetProgramiv(app->shader_prog, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(app->shader_prog, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(app->shader_prog, maxLength, &maxLength, &infoLog[0]);
        glDeleteProgram(app->shader_prog);
        std::cerr << "Linking shader program failed!" << std::endl;
        std::cerr << std::string(begin(infoLog), end(infoLog)) << std::endl;
        return;
    }

    // finally, assign (our only) program as the currently-active one
    glUseProgram(app->shader_prog);
    return;
}

void thirsty::newGeometries(Application* app) {
    // populate vertices and indices data
    app->verts = {
        -1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 1.0f
    };
    app->indices = {
        0, 1, 2, 0, 2, 3
    };

    // populate vertex and element buffers
    glGenBuffers(1, &app->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, app->vbo);
    glBufferData(GL_ARRAY_BUFFER, app->verts.size() * sizeof(GLfloat), app->verts.data(), GL_STATIC_DRAW);
    glGenBuffers(1, &app->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, app->indices.size() * sizeof(GLint), app->indices.data(), GL_STATIC_DRAW);

    // bind vertex position, texture coordinates
    GLint pos_attr_loc = glGetAttribLocation(app->shader_prog, "aXY");
    glVertexAttribPointer(pos_attr_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0); // 2 values; stride 4; offset 0
    glEnableVertexAttribArray(pos_attr_loc);
    GLint tex_attr_loc = glGetAttribLocation(app->shader_prog, "aUV");
    glVertexAttribPointer(tex_attr_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat))); // 2 values; stride 4; offset 0
    glEnableVertexAttribArray(tex_attr_loc);
}

void thirsty::newMaterials(Application* app) {
    // load logo
    app->logo_rgba = IMG_Load("logo.png");

    // transcribe raw image bytes into uniform texture buffer
    glGenTextures(1, &app->tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, app->tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glUniform1i(glGetUniformLocation(app->shader_prog, "uTexture"), 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // define texture sampling parameters; copy
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, app->logo_rgba->pixels);
}

void thirsty::loopRender(Application* app) {
    thirsty::render(app->renderer, app->scene, app->camera);
}

void thirsty::loopEvent(Application* app) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
            app->is_running = false;
            break;
        }
    }
}

const char* thirsty::getSource(const char* path) {
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
