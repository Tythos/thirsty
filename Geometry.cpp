/**
 * Geometry.cpp
 */

#include "thirsty.hpp"

void thirsty::newGeometry(thirsty::Geometry* geometry) {
    // initialize and bind vertex arrays
    glGenVertexArrays(1, &(geometry->vao));
    glBindVertexArray(geometry->vao);

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
