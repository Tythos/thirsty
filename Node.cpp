/**
 * Node.cpp
 */

#include "thirsty.hpp"

void thirsty::newNode(thirsty::Node* node) {
    newShaders(node);
    newGeometries(node);
    newMaterials(node);
}

void thirsty::freeNode(thirsty::Node* node) {
    freeMaterials(node);
    freeGeometries(node);
    freeShaders(node);
}

void thirsty::newShaders(thirsty::Node* node) {
    // status/buffers and vertex arrays
    GLint status;
    char err_buf[512];
    glGenVertexArrays(1, &(node->vao));
    glBindVertexArray(node->vao);

    // compile vertex shader
    node->vert_shader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexSource = getSource("basic.v.glsl");
    glShaderSource(node->vert_shader, 1, &vertexSource, NULL);
    glCompileShader(node->vert_shader);
    glGetShaderiv(node->vert_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog(node->vert_shader, sizeof(err_buf), NULL, err_buf);
        err_buf[sizeof(err_buf)-1] = '\0';
        std::cerr << "Compiling vertex shader failed!" << std::endl;
        std::cerr << err_buf << std::endl;
        return;
    }
    delete[] vertexSource;

    // compile fragment shader
    node->frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentSource = getSource("basic.f.glsl");
    glShaderSource(node->frag_shader, 1, &fragmentSource, NULL);
    glCompileShader(node->frag_shader);
    glGetShaderiv(node->frag_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog(node->frag_shader, sizeof(err_buf), NULL, err_buf);
        err_buf[sizeof(err_buf)-1] = '\0';
        std::cerr << "Compiling fragment shader failed!" << std::endl;
        std::cerr << err_buf << std::endl;
        return;
    }
    delete[] fragmentSource;

    // link shader program
    node->shader_prog = glCreateProgram();
    glAttachShader(node->shader_prog, node->vert_shader);
    glAttachShader(node->shader_prog, node->frag_shader);
    glBindFragDataLocation(node->shader_prog, 0, "oRGBA");
    glLinkProgram(node->shader_prog);
    GLint isLinked = 0;
    glGetProgramiv(node->shader_prog, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(node->shader_prog, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(node->shader_prog, maxLength, &maxLength, &infoLog[0]);
        glDeleteProgram(node->shader_prog);
        std::cerr << "Linking shader program failed!" << std::endl;
        std::cerr << std::string(begin(infoLog), end(infoLog)) << std::endl;
        return;
    }

    // finally, assign program as active
    glUseProgram(node->shader_prog);
    return;
}

void thirsty::newGeometries(thirsty::Node* node) {
    // populate vertices and indices data
    node->verts = {
        -1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 1.0f
    };
    node->indices = {
        0, 1, 2, 0, 2, 3
    };

    // populate vertex and element buffers
    glGenBuffers(1, &node->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, node->vbo);
    glBufferData(GL_ARRAY_BUFFER, node->verts.size() * sizeof(GLfloat), node->verts.data(), GL_STATIC_DRAW);
    glGenBuffers(1, &node->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, node->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, node->indices.size() * sizeof(GLint), node->indices.data(), GL_STATIC_DRAW);

    // bind vertex position, texture coordinates
    GLint pos_attr_loc = glGetAttribLocation(node->shader_prog, "aXY");
    glVertexAttribPointer(pos_attr_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0); // 2 values; stride 4; offset 0
    glEnableVertexAttribArray(pos_attr_loc);
    GLint tex_attr_loc = glGetAttribLocation(node->shader_prog, "aUV");
    glVertexAttribPointer(tex_attr_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat))); // 2 values; stride 4; offset 2
    glEnableVertexAttribArray(tex_attr_loc);
}

void thirsty::newMaterials(thirsty::Node* node) {
    // load logo
    node->logo_rgba = IMG_Load("logo.png");

    // transcribe raw image bytes into uniform texture buffer
    glGenTextures(1, &node->tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, node->tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glUniform1i(glGetUniformLocation(node->shader_prog, "uTexture"), 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // define texture sampling parameters; copy
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, node->logo_rgba->pixels);
}

void thirsty::freeMaterials(Node* node) {
    glDeleteTextures(1, &node->tex);
    SDL_FreeSurface(node->logo_rgba);
    node->logo_rgba = NULL;
}

void thirsty::freeGeometries(Node* node) {
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &node->ebo);
    glDeleteBuffers(1, &node->vbo);
    glDeleteVertexArrays(1, &node->vao);
}

void thirsty::freeShaders(Node* node) {
    glUseProgram(0);
    glDetachShader(node->shader_prog, node->vert_shader);
    glDetachShader(node->shader_prog, node->frag_shader);
    glDeleteProgram(node->shader_prog);
    glDeleteShader(node->vert_shader);
    glDeleteShader(node->frag_shader);
}
