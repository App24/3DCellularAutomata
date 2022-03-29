#include "window/Window.h"
#include "window/Events.h"
#include "window/Camera.h"

#include "graphics/Shader.h"
#include "graphics/Mesh.h"
#include "graphics/VoxelRenderer.h"

#include "CellularAutomata.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "graphics/LineBatch.h"

#include "imguiExtra.h"

int main() {

	Window::initialise(1280, 720, "3D Cellular Automata");
	Events::initialise();

	Shader* shader = load_shader("res/main.vs", "res/main.fs");
	Shader* linesShader = load_shader("res/lines.vs", "res/lines.fs");

	int width = 40;
	int height = 40;
	int depth = 40;

	Camera* camera = new Camera(glm::vec3(width/2.f, height/2.f, depth * 2.f), glm::radians(60.f));

	glm::mat4 model = glm::mat4(1.f);

	model = glm::translate(model, glm::vec3(0, 0, 0));

	float lastTime = glfwGetTime();
	float delta = 0.0f;

	CellularRules rules;

	if (!generate_rule("4/4/5/M", &rules)) {
		return -1;
	}

	std::vector<float> cellColors;
	for (size_t i = 1; i <= rules.states; i++)
	{
		cellColors.push_back(i/(float)rules.states);
		cellColors.push_back(i/(float)rules.states);
		cellColors.push_back(i/(float)rules.states);
	}

	bool cube = true;
	int cubeSize = 7;

	VoxelRenderer* renderer = new VoxelRenderer(1024 * 1024 * 8);
	CellularAutomata* automata = new CellularAutomata(width, height, depth, cellColors, rules, cube, cubeSize);

	LineBatch* lineBatch = new LineBatch(4096);

	Mesh* mesh = renderer->render(automata);

	float time = 0;

	bool paused = false;
	bool unlocked = false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(Window::getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	int simulationSpeed = 5;

	float movementSpeed = 10;

	char rule[512]={0};

	memcpy(rule, rules.rule.c_str(), rules.rule.size());

	while (!Window::isShouldClose()) {
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		float currentTime = glfwGetTime();
		delta = currentTime - lastTime;
		lastTime = currentTime;

		if (!ImGui::IsAnyItemActive()) {
			if (Events::keyPressed(GLFW_KEY_ESCAPE)) {
				Window::setShouldClose(true);
			}

			if (Events::keyHeld(GLFW_KEY_Q)) {
				camera->rotate(0, 2 * delta, 0);
			}
			else if (Events::keyHeld(GLFW_KEY_E)) {
				camera->rotate(0, -2 * delta, 0);
			}

			if (Events::keyHeld(GLFW_KEY_R)) {
				camera->rotate(2 * delta, 0, 0);
			}
			else if (Events::keyHeld(GLFW_KEY_T)) {
				camera->rotate(-2 * delta, 0, 0);
			}

			if (Events::keyHeld(GLFW_KEY_W)) {
				camera->position += camera->front * movementSpeed * delta;
			}
			else if (Events::keyHeld(GLFW_KEY_S)) {
				camera->position -= camera->front * movementSpeed * delta;
			}

			if (Events::keyHeld(GLFW_KEY_A)) {
				camera->position -= camera->right * movementSpeed * delta;
			}
			else if (Events::keyHeld(GLFW_KEY_D)) {
				camera->position += camera->right * movementSpeed * delta;
			}

			if (Events::keyHeld(GLFW_KEY_LEFT_SHIFT)) {
				camera->position -= camera->up * movementSpeed * delta;
			}
			else if (Events::keyHeld(GLFW_KEY_SPACE)) {
				camera->position += camera->up * movementSpeed * delta;
			}

			if (Events::keyPressed(GLFW_KEY_P)) {
				paused = !paused;
			}
		}

		if (time >= 1/(float)simulationSpeed || (unlocked && !paused)) {
			automata->simulate();
			mesh = renderer->render(automata);
			time = 0;
		}

		if (ImGui::Begin("Color")) {
			for (size_t i = 0; i < rules.states*3; i+=3)
			{
				if (ImGui::CollapsingHeader(("State: " + std::to_string(i/3)).c_str())) {
					float colors[3] = { cellColors[i], cellColors[i+1], cellColors[i+2] };
					if (ImGui::ColorPicker("Cell Color", colors)) {
						cellColors[i+0] = colors[0];
						cellColors[i+1] = colors[1];
						cellColors[i+2] = colors[2];
						automata->colors = cellColors;
						mesh = renderer->render(automata);
					}
				}
			}

			ImGui::End();
		}

		if (ImGui::Begin("Simulation Stats")) {

			if (!unlocked)
				ImGui::Text("Simulation Speed: %i", simulationSpeed);
			else
				ImGui::Text("Simulation Speed: Unlocked");

			ImGui::Text("Current Rule: %s", rules.rule.c_str());
			ImGui::Text("Delta Time: %f", delta);

			ImGui::Text("Current Width: %i", automata->getWidth());
			ImGui::Text("Current Height: %i", automata->getHeight());
			ImGui::Text("Current Depth: %i", automata->getDepth());
			ImGui::Text("Current Volume: %i", automata->getWidth() * automata->getHeight() * automata->getDepth());

			ImGui::End();
		}

		if (ImGui::Begin("Simulation Settings")) {

			if (ImGui::InputFloat("Movement Speed", &movementSpeed)) {
				if (movementSpeed < 1)
					movementSpeed = 1;
			}

			if (ImGui::InputInt("Threads", &CellularAutomata::threadCount)) {
				if (CellularAutomata::threadCount < 0)
					CellularAutomata::threadCount = 1;
				automata->updateThreads();
			}

			if (ImGui::InputInt("Speed", &simulationSpeed)) {
				if (simulationSpeed < 0)
					simulationSpeed = 1;
			}

			ImGui::Checkbox("Unlocked", &unlocked);

			if (ImGui::Button(paused ? "Unpause" : "Pause")) {
				paused = !paused;
			}

			if (paused) {
				if (ImGui::Button("Next Frame")) {
					time = 1;
				}
			}

			ImGui::Checkbox("Wrap", &automata->wrap);

			ImGui::End();
		}

		if (ImGui::Begin("Automata Settings")) {
			ImGui::InputText("Rule", rule, 512);

			if (ImGui::InputInt("Width", &width)) {
				if (width < 0)
					width = 1;
			}

			if (ImGui::InputInt("Height", &height)) {
				if (height < 0)
					height = 1;
			}

			if (ImGui::InputInt("Depth", &depth)) {
				if (depth < 0)
					depth = 1;
			}

			ImGui::Checkbox("Cube", &cube);

			if (ImGui::InputInt("Cube Size", &cubeSize)) {
				if (cubeSize < 0)
					cubeSize = 1;
			}

			if (ImGui::Button("Restart")) {
				if (generate_rule(rule, &rules)) {
					if (cellColors.size() / 3 != rules.states) {
						cellColors.clear();
						for (size_t i = 1; i <= rules.states; i++)
						{
							cellColors.push_back(i / (float)rules.states);
							cellColors.push_back(i / (float)rules.states);
							cellColors.push_back(i / (float)rules.states);
						}
					}
					automata = new CellularAutomata(width, height, depth, cellColors, rules, cube, cubeSize);
					mesh = renderer->render(automata);
					time = 0;

					camera->position = vec3(width / 2.f, height / 2.f, depth * 2.f);
					camera->rotation = glm::mat4(1.f);
					camera->updateVectors();
				}
			}

			ImGui::End();
		}

		shader->use();
		shader->uniformMatrix("proj", camera->getProjection());
		shader->uniformMatrix("view", camera->getView());
		shader->uniformMatrix("model", model);
		mesh->draw(GL_TRIANGLES);

		linesShader->use();
		linesShader->uniformMatrix("proj", camera->getProjection()* camera->getView());
		glLineWidth(2.f);

		{
			int lineWidth = automata->getWidth();
			int lineHeight = automata->getHeight();
			int lineDepth = automata->getDepth();
			lineBatch->line(0, 0, 0,					0, lineHeight, 0,					0, 1, 0, 1);
			lineBatch->line(lineWidth, 0, 0,			lineWidth, lineHeight, 0,			0, 1, 0, 1);
			lineBatch->line(0, 0, lineDepth,			0, lineHeight, lineDepth,			0, 1, 0, 1);
			lineBatch->line(lineWidth, 0, lineDepth,	lineWidth, lineHeight, lineDepth,	0, 1, 0, 1);

			lineBatch->line(lineWidth, lineHeight, 0,	lineWidth, lineHeight, lineDepth,	0, 1, 0, 1);
			lineBatch->line(0, lineHeight, 0,			0, lineHeight, lineDepth,			0, 1, 0, 1);
			lineBatch->line(0, lineHeight, lineDepth,	lineWidth, lineHeight, lineDepth,	0, 1, 0, 1);
			lineBatch->line(0, lineHeight, 0,			lineWidth, lineHeight, 0,			0, 1, 0, 1);

			lineBatch->line(lineWidth, 0, 0,			lineWidth, 0, lineDepth,			0, 1, 0, 1);
			lineBatch->line(0, 0, 0,					0, 0, lineDepth,					0, 1, 0, 1);
			lineBatch->line(0, 0, lineDepth,			lineWidth, 0, lineDepth,			0, 1, 0, 1);
			lineBatch->line(0, 0, 0,					lineWidth, 0, 0,					0, 1, 0, 1);
		}

		lineBatch->render();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		Window::swapBuffers();
		Events::pollEvents();

		if(!paused)
		time += delta;
	}

	delete automata;
	delete renderer;
	delete lineBatch;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	Events::finalise();
	Window::terminate();

	return 0;
}