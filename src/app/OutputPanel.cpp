#include "OutputPanel.h"
#include <glad/glad.h>
#include <iostream>
#include <cstdint>
#include <glm/gtx/matrix_decompose.hpp>

#define ORBIT_SENSITIVITY 0.01f
#define PAN_SENSITIVITY 0.003f
#define MAX_DISTANCE 20.0f
#define MIN_DISTANCE 1.0f

#define CAMERA_LIMIT 1.565

void OutputPanel::UpdateCamera()
{
	m_camera->SetPosition(m_gimbal.GetPosition() + m_gimbal.Forward() * m_distance);
	m_camera->LookAt(m_gimbal.GetPosition(), glm::vec3(0.0, 1.0, 0.0));
}

void OutputPanel::DrawSkeleton(const Joint* joint, const glm::mat4& parentMatrix, int drawLevel, int currentLevel)
{
	glm::mat4 currentMatrix = parentMatrix * joint->transform;

	if (currentLevel >= drawLevel - 1)
	{
		glm::mat4 jointModelMatrix = m_camera->GetMatrix() * currentMatrix;
		m_shader.SetUniformMatrix4fv("u_Mat", (float*)(&jointModelMatrix));
		m_jointModel.Draw();
	}

	if (currentLevel >= drawLevel)
	{
		glm::quat rotation;
		glm::vec3 translation, skew, scale;
		glm::vec4 perspective;
		glm::decompose(joint->transform, scale, rotation, translation, skew, perspective);
		float boneLength = glm::length(translation);
		glm::mat4 boneModelMatrix =
			m_camera->GetMatrix() *
			parentMatrix *
			glm::scale(glm::mat4(1.0f), glm::vec3(boneLength, boneLength, boneLength));
		m_shader.SetUniformMatrix4fv("u_Mat", (float*)(&boneModelMatrix));
		m_boneModel.Draw();
	}

	for (const Joint* j : joint->children)
		DrawSkeleton(j, currentMatrix, drawLevel, currentLevel + 1);
}

OutputPanel::OutputPanel(const std::string& name, const glm::vec3& clearColor) : Panel(name, clearColor)
{
	m_boneModel.CreateFromOBJ("assets/bone.obj");
	m_jointModel.CreateFromOBJ("assets/joint.obj");

	m_skeleton = new Skeleton("assets/ag.fbx");

	CameraSpecs ss;
	ss.perspective = true;
	m_camera = new Camera(ss);
	m_distance = 3.19f;
	m_gimbal.SetRotation(glm::fquat(m_gimbalRot));
	UpdateCamera();

	m_shader.CreateFromFiles("assets/shaders/vert.glsl", "assets/shaders/frag.glsl");
	m_shader.Bind();
	float theColor[4] = { 0.1, 0.1, 0.9, 1.0 };
	m_shader.SetUniform4fv("u_Color", theColor);
}

OutputPanel::~OutputPanel()
{
	delete m_camera;
	delete m_skeleton;
}

void OutputPanel::OnResize()
{
	m_camera->m_specs.aspectRatio = m_size.x / m_size.y;
}

void OutputPanel::HandleInput(const ImGuiIO& io, const glm::vec2& relativeMousePos)
{
	m_distance -= io.MouseWheel;
	if (m_distance > MAX_DISTANCE)
		m_distance = MAX_DISTANCE;
	else if (m_distance < MIN_DISTANCE)
		m_distance = MIN_DISTANCE;

	if (io.MouseDown[2])
	{
		if (io.KeyShift)
		{
			m_gimbal.SetPosition(
				m_gimbal.GetPosition() +
				m_camera->Up() * io.MouseDelta.y * m_distance * PAN_SENSITIVITY -
				m_camera->Right() * io.MouseDelta.x * m_distance * PAN_SENSITIVITY
			);
		}
		else
		{
			m_gimbalRot.y -= io.MouseDelta.x * ORBIT_SENSITIVITY;
			m_gimbalRot.x += io.MouseDelta.y * ORBIT_SENSITIVITY;

			if (m_gimbalRot.x > CAMERA_LIMIT)
				m_gimbalRot.x = CAMERA_LIMIT;
			else if (m_gimbalRot.x < -CAMERA_LIMIT)
				m_gimbalRot.x = -CAMERA_LIMIT;

			m_gimbal.SetRotation(glm::fquat(m_gimbalRot));
		}
	}
	UpdateCamera();
}

void OutputPanel::Draw()
{
	glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	m_camera->Bind();
	m_camera->ComputeMatrices();

	m_shader.Bind();

	DrawSkeleton(m_skeleton->rootJoint, glm::mat4(1.0f));
}
