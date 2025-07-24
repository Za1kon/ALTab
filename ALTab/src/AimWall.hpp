#pragma once
#include "Scene.hpp"
#include "Wall.hpp"

class AimTrainer : public Scene {
public:
    AimTrainer();
    ~AimTrainer();

    void Init() override;
    void Update(float deltaTime) override;
    void Render(const glm::mat4& projection, const glm::mat4& view) override;

private:
    Wall* wall;
};