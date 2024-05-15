/**
 * Node.cpp
 */

#include "thirsty.hpp"

void thirsty::newNode(thirsty::Node* node) {
    node->geometry = new Geometry();
    node->material = new Material();
    newGeometry(node->geometry);
    newMaterial(node->material);
}

void thirsty::freeNode(thirsty::Node* node) {
    freeMaterial(node->material);
    freeGeometry(node->geometry);
}
