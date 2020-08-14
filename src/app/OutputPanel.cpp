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

void OutputPanel::DrawSkeleton(const Skeleton& skeleton)
{
	for (const RootBone& rb : skeleton.m_rootBones)
	{
		DrawBoneTree(rb.bone, glm::translate(glm::mat4(1.0f), rb.offset));
	}
}

void OutputPanel::DrawBoneTree(const Bone* currentBone, const glm::mat4& parentMatrix)
{
	glm::mat4 jointModelMatrix = m_camera->GetMatrix() * parentMatrix;
	m_shader.SetUniformMatrix4fv("u_Mat", (float*)(&jointModelMatrix));
	m_jointModel.Draw();

	glm::mat4 withBoneTransform = parentMatrix * currentBone->GetMatrix();

	glm::mat4 boneModelMatrix =
		m_camera->GetMatrix() *
		parentMatrix *
		((glm::mat4)currentBone->rotation) *
		glm::scale(glm::mat4(1.0f), glm::vec3(currentBone->size, currentBone->size, currentBone->size));

	m_shader.SetUniformMatrix4fv("u_Mat", (float*)(&boneModelMatrix));
	m_boneModel.Draw();
	for (Bone* b : currentBone->children)
		DrawBoneTree(b, withBoneTransform);
}

OutputPanel::OutputPanel(const std::string& name, const glm::vec3& clearColor) : Panel(name, clearColor)
{
	m_boneModel.CreateFromOBJ("assets/bone.obj");
	m_jointModel.CreateFromOBJ("assets/joint.obj");

	m_skeleton = new Skeleton();
	m_assimpSkeleton = new Skeleton("assets/ag.bvh");

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
	delete m_assimpSkeleton;
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

	DrawSkeleton(*m_assimpSkeleton);
}
