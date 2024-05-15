/**
 * Geometry.cpp
 */

#include "thirsty.hpp"

void thirsty::newGeometry(thirsty::Geometry* geometry) {
    glGenVertexArrays(1, &(geometry->vao));
    glBindVertexArray(geometry->vao);

    // populate vertices and indices data
        geometry->verts = {
        -1.0f, -1.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 1.0f
    };
    geometry->indices = {
        0, 1, 2, 0, 2, 3
    };

    // populate vertex and element buffers
    glGenBuffers(1, &geometry->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, geometry->vbo);
    glBufferData(GL_ARRAY_BUFFER, geometry->verts.size() * sizeof(GLfloat), geometry->verts.data(), GL_STATIC_DRAW);
    glGenBuffers(1, &geometry->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry->indices.size() * sizeof(GLint), geometry->indices.data(), GL_STATIC_DRAW);
}

void thirsty::freeGeometry(Geometry* geometry) {
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &geometry->ebo);
    glDeleteBuffers(1, &geometry->vbo);
    glDeleteVertexArrays(1, &geometry->vao);
}
