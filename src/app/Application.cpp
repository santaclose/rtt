#include "Application.h"
#include "InputPanel.h"
#include "OutputPanel.h"
#include "GraphPanel.h"

InputPanel* inputPanel;
GraphPanel* graphPanel;
OutputPanel* outputPanel;
glm::vec3 test = glm::vec3(0.0f);
glm::vec3 test2 = glm::vec3(0.0f);

void Application::Init()
{
	inputPanel = new InputPanel("Input", glm::vec3(0.7, 0.7, 0.7));
	graphPanel = new GraphPanel("Graph", glm::vec3(0.7, 0.7, 0.7));
	outputPanel = new OutputPanel("Output", glm::vec3(0.7, 0.7, 0.7));
}

void Application::OnUpdate(const ImGuiIO& io)
{
	for (Panel* p : Panel::all)
		p->ImGuiCall(io);

	ImGui::Begin("Debug");
	ImGui::SliderFloat3("test", &test.x, -3.1415, 3.1415);
	ImGui::SliderFloat3("test2", &test2.x, -3.1415, 3.1415);
	ImGui::End();

	outputPanel->m_skeleton->m_rootBone->children[0]->children[0]->animOffsetMatrix = (glm::mat4) glm::fquat(test);
	outputPanel->m_skeleton->m_rootBone->children[0]->animOffsetMatrix = (glm::mat4) glm::fquat(test2);
}

void Application::Terminate()
{
	delete inputPanel;
	delete graphPanel;
	delete outputPanel;
}
