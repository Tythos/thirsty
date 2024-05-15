/**
 * Material.cpp
 */

#include "thirsty.hpp"

void thirsty::newMaterial(Material* material) {
    // status/buffers and vertex arrays
    GLint status;
    char err_buf[512];

    // load texture as SDL surface
    material->texture = IMG_Load("logo.png");

    // compile vertex shader
    material->vert_shader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexSource = getSource("basic.v.glsl");
    glShaderSource(material->vert_shader, 1, &vertexSource, NULL);
    glCompileShader(material->vert_shader);
    glGetShaderiv(material->vert_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog(material->vert_shader, sizeof(err_buf), NULL, err_buf);
        err_buf[sizeof(err_buf)-1] = '\0';
        std::cerr << "Compiling vertex shader failed!" << std::endl;
        std::cerr << err_buf << std::endl;
        return;
    }
    delete[] vertexSource;

    // compile fragment shader
    material->frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentSource = getSource("basic.f.glsl");
    glShaderSource(material->frag_shader, 1, &fragmentSource, NULL);
    glCompileShader(material->frag_shader);
    glGetShaderiv(material->frag_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog(material->frag_shader, sizeof(err_buf), NULL, err_buf);
        err_buf[sizeof(err_buf)-1] = '\0';
        std::cerr << "Compiling fragment shader failed!" << std::endl;
        std::cerr << err_buf << std::endl;
        return;
    }
    delete[] fragmentSource;

    // link shader program
    material->shader_prog = glCreateProgram();
    glAttachShader(material->shader_prog, material->vert_shader);
    glAttachShader(material->shader_prog, material->frag_shader);
    glBindFragDataLocation(material->shader_prog, 0, "oRGBA");
    glLinkProgram(material->shader_prog);
    GLint isLinked = 0;
    glGetProgramiv(material->shader_prog, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(material->shader_prog, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(material->shader_prog, maxLength, &maxLength, &infoLog[0]);
        glDeleteProgram(material->shader_prog);
        std::cerr << "Linking shader program failed!" << std::endl;
        std::cerr << std::string(begin(infoLog), end(infoLog)) << std::endl;
        return;
    }

    // bind vertex position, texture coordinates
    GLint pos_attr_loc = glGetAttribLocation(material->shader_prog, "aXY");
    glVertexAttribPointer(pos_attr_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0); // 2 values; stride 4; offset 0
    glEnableVertexAttribArray(pos_attr_loc);
    GLint tex_attr_loc = glGetAttribLocation(material->shader_prog, "aUV");
    glVertexAttribPointer(tex_attr_loc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat))); // 2 values; stride 4; offset 2
    glEnableVertexAttribArray(tex_attr_loc);

    // transcribe surface into uniform buffer
    glGenTextures(1, &material->tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material->tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glUniform1i(glGetUniformLocation(material->shader_prog, "uTexture"), 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // define texture sampling parameters; copy
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, material->texture->pixels);

    // finally, assign program as active
    glUseProgram(material->shader_prog);
}

void thirsty::freeMaterial(Material* material) {
    // free texture data
    glDeleteTextures(1, &material->tex);
    SDL_FreeSurface(material->texture);
    material->texture = NULL;

    // detach and delete shaders
    glUseProgram(0);
    glDetachShader(material->shader_prog, material->vert_shader);
    glDetachShader(material->shader_prog, material->frag_shader);
    glDeleteProgram(material->shader_prog);
    glDeleteShader(material->vert_shader);
    glDeleteShader(material->frag_shader);
}
