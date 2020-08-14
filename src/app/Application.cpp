#include "Application.h"
#include "InputPanel.h"
#include "OutputPanel.h"
#include "GraphPanel.h"

InputPanel* inputPanel;
GraphPanel* graphPanel;
OutputPanel* outputPanel;

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
}

void Application::Terminate()
{
	delete inputPanel;
	delete graphPanel;
	delete outputPanel;
}
