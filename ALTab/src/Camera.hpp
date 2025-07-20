#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Camera {
public:
    Camera(const glm::vec3& position,
        const glm::vec3& target,
        const glm::vec3& up,
        float fov,
        float aspect,
        float nearPlane,
        float farPlane);

    void UpdateViewMatrix();
    void UpdateProjectionMatrix(float aspectRatio);

    const glm::mat4& GetView() const;
    const glm::mat4& GetProjection() const;

    void SetPosition(const glm::vec3& pos);
    void SetTarget(const glm::vec3& tgt);
    const glm::vec3& GetPosition() const { return position; }

private:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float fov, aspect, nearPlane, farPlane;
    glm::mat4 view;
    glm::mat4 projection;
};

#endif // CAMERA_HPP