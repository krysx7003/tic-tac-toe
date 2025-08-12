#include <cstdio>
#include <cstdlib>
#include <string>

#include "include/glad/glad.h"
#include "include/imgui/backends/imgui_impl_glfw.h"
#include "include/imgui/backends/imgui_impl_opengl3.h"
#include "include/imgui/imgui.h"
#include <GLFW/glfw3.h>

#include "game.h"
#include "include/utils/resource_manager.h"

using namespace std;

Game game;
double currX, currY;
int click_count = 0;

void render(GLFWwindow *window);
void render_debug_frame(GLFWwindow *window);
static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
bool isValidInput(char c);

int main(int argc, char *argv[]) {
	bool gui = true;
	if (argc >= 2) {
		string gui_arg = argv[1];
		if (gui_arg == "false") {
			gui = false;
		}
	}

	if (gui) {
		if (!glfwInit())
			return -1;

		GLFWwindow *window = glfwCreateWindow(600, 600, "Tic-Tac-Toe", NULL, NULL);
		if (!window) {
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();
			return -1;
		}

		ResourceManager::LoadShader("../shaders/tile.vs", "../shaders/tile.frag", "tile");

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 130");
		game = Game(gui);

		glfwSetCursorPosCallback(window, cursor_position_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);

		while (!glfwWindowShouldClose(window)) {
			render(window);
		}
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(window);

		glfwTerminate();
	} else {
		game = Game();

		while (true) {
			game.start();

			char c;
			printf("Restart (y/n): ");
			scanf(" %c", &c);

			while (!isValidInput(c)) {
				printf("Invalid input. Restart (y/n): ");
				scanf(" %c", &c);
			}

			if (c == 'y')
				game.restart();
			else
				break;
		}
	}
	return 0;
}

bool isValidInput(char c) {
	if (c == 'y' || c == 'n')
		return true;

	return false;
}

void render(GLFWwindow *window) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	game.board.render();

	render_debug_frame(window);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void render_debug_frame(GLFWwindow *window) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Debug");

	ImGui::Text("Click count: %d", click_count);
	ImGui::Text("Position x: %.2f, y: %.2f", currX, currY);
	ImGui::Text("Clicked tile: %d", game.getLastTile());
	ImGui::Text("Current player: %c", game.getPlayer());

	char *state = game.board.getTilesState();
	ImGui::Text("| %c | %c | %c |\n-------------", state[0], state[1], state[2]);
	ImGui::Text("| %c | %c | %c |\n-------------", state[3], state[4], state[5]);
	ImGui::Text("| %c | %c | %c |\n-------------", state[6], state[7], state[8]);

	if (!game.active) {
		ImGui::Text("Game ended winner: %c", game.getWinner());
	}

	if (ImGui::Button("Restart")) {
		game.restart();
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
	if (io.WantCaptureMouse)
		return;

	currX = (xpos / 300) - 1;
	currY = -((ypos / 300) - 1);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
	if (io.WantCaptureMouse)
		return;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		click_count++;
		game.chosenTile(currX, currY);
	}
}
