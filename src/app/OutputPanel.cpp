#include "OutputPanel.h"
#include <glad/glad.h>
#include <iostream>
#include <cstdint>

#define SENSITIVITY 0.01f
#define MAX_DISTANCE 20.0f
#define MIN_DISTANCE 1.0f

#define CAMERA_LIMIT 1.565

void OutputPanel::UpdateCamera()
{
	m_camera->SetPosition(m_gimbal.Forward() * m_distance);
	m_camera->LookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
}

void OutputPanel::DrawSkeleton(const Bone* currentBone, glm::mat4 parentMatrix)
{
	glm::mat4 currentMatrix = parentMatrix * currentBone->animOffsetMatrix * currentBone->offsetMatrix;

	glm::vec3 headPos = parentMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec3 tailPos = currentMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	float distance = glm::distance(headPos, tailPos);

	glm::mat4 modelMatrix =
		glm::translate(glm::mat4(1.0f), headPos) *
		((glm::mat4) glm::quatLookAt(glm::normalize(tailPos - headPos), glm::vec3(0.0f, 0.0f, -1.0f))) *
		glm::scale(glm::mat4(1.0f), glm::vec3(distance, distance, distance));

	{
		glm::mat4 uMatrix = m_camera->GetMatrix() * modelMatrix;
		m_shader.SetUniformMatrix4fv("u_Mat", (float*)(&uMatrix));
		m_boneModel.Draw();
	}
	{
		glm::mat4 uMatrix = m_camera->GetMatrix() * currentMatrix;
		m_shader.SetUniformMatrix4fv("u_Mat", (float*)(&uMatrix));
		m_jointModel.Draw();

	}
	for (Bone* b : currentBone->children)
		DrawSkeleton(b, currentMatrix);
}

OutputPanel::OutputPanel(const std::string& name, const glm::vec3& clearColor) : Panel(name, clearColor)
{
	m_boneModel.CreateFromOBJ("assets/bone.obj");
	m_jointModel.CreateFromOBJ("assets/joint.obj");

	m_skeleton = new Skeleton("assets/untitled.fbx");

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
		m_gimbalRot.y -= io.MouseDelta.x * SENSITIVITY;
		m_gimbalRot.x += io.MouseDelta.y * SENSITIVITY;

		if (m_gimbalRot.x > CAMERA_LIMIT)
			m_gimbalRot.x = CAMERA_LIMIT;
		else if (m_gimbalRot.x < -CAMERA_LIMIT)
			m_gimbalRot.x = -CAMERA_LIMIT;

		m_gimbal.SetRotation(glm::fquat(m_gimbalRot));
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

	DrawSkeleton(m_skeleton->m_rootBone, glm::mat4(1.0f));
}
