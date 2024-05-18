/**
 * Camera.cpp
 */

#include "thirsty.hpp"

void thirsty::newCamera(Camera* camera) {
    std::cout << "Camera initialized" << std::endl;
}

void thirsty::freeCamera(Camera* camera) {
    std::cout << "Camera deinitialized" << std::endl;
}

glm::mat4 thirsty::getViewMatrix(Camera* camera) {
    return glm::lookAt(camera->xyz, camera->xyz + camera->look, camera->up);
}

glm::mat4 thirsty::getProjectionMatrix(Camera* camera) {
    return glm::perspective(camera->fov_rad, camera->aspectRatio, camera->nearPlane, camera->farPlane);
}
