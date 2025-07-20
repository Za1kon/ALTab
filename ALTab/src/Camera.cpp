// Camera.cpp
#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up,
    float fov, float aspect, float nearPlane, float farPlane)
    : position(position), target(target), up(up),
    fov(fov), aspect(aspect), nearPlane(nearPlane), farPlane(farPlane)
{
    UpdateViewMatrix();
    UpdateProjectionMatrix(aspect);
}

void Camera::UpdateViewMatrix() {
    view = glm::lookAt(position, target, up);
}

void Camera::UpdateProjectionMatrix(float aspectRatio) {
    aspect = aspectRatio;
    projection = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}

const glm::mat4& Camera::GetView() const {
    return view;
}

const glm::mat4& Camera::GetProjection() const {
    return projection;
}

void Camera::SetPosition(const glm::vec3& pos) {
    position = pos;
    UpdateViewMatrix();
}

void Camera::SetTarget(const glm::vec3& tgt) {
    target = tgt;
    UpdateViewMatrix();
}
