#include "InputPanel.h"
#include <glad/glad.h>
#include <iostream>
#include <cstdint>

#define SENSITIVITY 0.01f
#define MAX_DISTANCE 7.0f
#define MIN_DISTANCE 2.0f

#define CAMERA_LIMIT 1.565

void InputPanel::UpdateCamera()
{
	m_camera->SetPosition(m_gimbal.Forward() * m_distance);
	m_camera->LookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
}

InputPanel::InputPanel(const std::string& name, const glm::vec3& clearColor) : Panel(name, clearColor)
{
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

	// 1. bind Vertex Array Object
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void InputPanel::OnResize()
{
	m_camera->m_specs.aspectRatio = m_size.x / m_size.y;
}

void InputPanel::HandleInput(const ImGuiIO& io, const glm::vec2& relativeMousePos)
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

void InputPanel::Draw()
{
	glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	m_camera->Bind();
	m_camera->ComputeMatrices();

	m_shader.Bind();
	m_shader.SetUniformMatrix4fv("u_Mat", (float*)(&(m_camera->GetMatrix())));
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6);
}
