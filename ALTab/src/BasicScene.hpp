// ==== Actualización de BasicScene ====
// File: BasicScene.hpp
#pragma once
#include "Scene.hpp"
#include "Shader.hpp"
#include "Platform.hpp"
#include "AimWallManager.hpp"
#include "ConfigWall.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <GLFW/glfw3.h>

class BasicScene : public Scene {
public:
    BasicScene(float terrainSize, int terrainRes);
    ~BasicScene();

    void Init() override;
    void Update(float delta) override;
    void Render(const glm::mat4& projection, const glm::mat4& view) override;
    void HandleInput();

private:
    float terrainSize;
    int terrainRes;

    Shader* platformShader;
    Platform* platform;
    GLFWwindow* window;

    std::unique_ptr<AimWallManager> aimWallManager_;
    std::unique_ptr<ConfigWall> configWall_;
    bool showAimWalls_ = false;

    BasicScene(const BasicScene&) = delete;
    BasicScene& operator=(const BasicScene&) = delete;
};