#pragma once
#include "../Panel.h"
#include "Shader.h"
#include "Camera.h"
#include "Skeleton.h"
#include "Model.h"

class OutputPanel : public Panel
{
public:
    Entity m_gimbal;
    glm::vec3 m_gimbalRot = glm::vec3(0.0, 3.14159265, 0.0);
    float m_distance = 4.0;
	Shader m_shader;
	Camera* m_camera;

    Skeleton* m_skeleton;

    Model m_boneModel;
    Model m_jointModel;

    void UpdateCamera();

    void DrawSkeleton(const Joint* joint, const glm::mat4& parentMatrix, int drawLevel = 2, int currentLevel = 0);

public:
	OutputPanel(const std::string& name, const glm::vec3& clearColor);
    ~OutputPanel();
	void OnResize();
	void HandleInput(const ImGuiIO& io, const glm::vec2& relativeMousePos);
	void Draw();
};