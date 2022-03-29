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

int main() {

	Window::initialise(1280, 720, "3D Cellular Automata");
	Events::initialise();

	Shader* shader = load_shader("res/main.vs", "res/main.fs");

	int width = 80;
	int height = 80;
	int depth = 80;

	Camera* camera = new Camera(glm::vec3(width/2.f, height/2.f, depth * 2.f), glm::radians(60.f));

	glm::mat4 model = glm::mat4(1.f);

	model = glm::translate(model, glm::vec3(0, 0, 0));

	float lastTime = glfwGetTime();
	float delta = 0.0f;

	CellularRules rules;

	if (!generate_rule("4/4/5/M", &rules)) {
		return -1;
	}

	VoxelRenderer* renderer = new VoxelRenderer(1024 * 1024 * 8);
	CellularAutomata* automata = new CellularAutomata(width, height, depth, vec3(1), rules);

	Mesh* mesh = renderer->render(automata);

	float time = 0;

	bool paused = false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(Window::getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	int simulationSpeed = 5;

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
				camera->position += camera->front * 10.f * delta;
			}
			else if (Events::keyHeld(GLFW_KEY_S)) {
				camera->position -= camera->front * 10.f * delta;
			}

			if (Events::keyHeld(GLFW_KEY_A)) {
				camera->position -= camera->right * 10.f * delta;
			}
			else if (Events::keyHeld(GLFW_KEY_D)) {
				camera->position += camera->right * 10.f * delta;
			}

			if (Events::keyHeld(GLFW_KEY_LEFT_SHIFT)) {
				camera->position -= camera->up * 10.f * delta;
			}
			else if (Events::keyHeld(GLFW_KEY_SPACE)) {
				camera->position += camera->up * 10.f * delta;
			}

			if (Events::keyPressed(GLFW_KEY_P)) {
				paused = !paused;
			}
		}

		if (time >= 1/(float)simulationSpeed) {
			automata->simulate();
			mesh = renderer->render(automata);
			time = 0;
		}

		if (ImGui::Begin("Simulation Stats")) {

			ImGui::Text("Simulation Speed: %i", simulationSpeed);
 			ImGui::Text("Current Rule: %s", rules.rule.c_str());
			ImGui::Text("Delta Time: %f", delta);

			ImGui::Text("Current Width: %i", automata->getWidth());
			ImGui::Text("Current Height: %i", automata->getHeight());
			ImGui::Text("Current Depth: %i", automata->getDepth());
			ImGui::Text("Current Volume: %i", automata->getWidth() * automata->getHeight() * automata->getDepth());

			ImGui::End();
		}

		if (ImGui::Begin("Simulation Settings")) {

			if (ImGui::InputInt("Speed", &simulationSpeed)) {
				if (simulationSpeed < 0)
					simulationSpeed = 1;
			}

			if (ImGui::Button(paused ? "Unpause" : "Pause")) {
				paused = !paused;
			}

			if (paused) {
				if (ImGui::Button("Next Frame")) {
					time = 1;
				}
			}

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

			if (ImGui::Button("Restart")) {
				if (generate_rule(rule, &rules)) {
					automata = new CellularAutomata(width, height, depth, automata->cellColor, rules);
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

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		Window::swapBuffers();
		Events::pollEvents();

		if(!paused)
		time += delta;
	}

	delete automata;
	delete renderer;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	Events::finalise();
	Window::terminate();

	return 0;
}