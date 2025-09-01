#include <cstdio>
#include <cstdlib>
#include <string>

#include "thirdparty/glad/glad.h"
#include "thirdparty/imgui/backends/imgui_impl_glfw.h"
#include "thirdparty/imgui/backends/imgui_impl_opengl3.h"
#include "thirdparty/imgui/imgui.h"
#include <GLFW/glfw3.h>

#include "game.h"
#include "utils/resource_manager.h"

using json = nlohmann::json;
using namespace std;

Game game;
double currX, currY;
int click_count = 0;

void render(GLFWwindow *window);
void render_debug_frame(GLFWwindow *window);
static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
bool isValidInput(char c);

int main() {

	bool gui = ResourceManager::LoadConfig()["gui"];

	if (gui) {
		if (!glfwInit()) {
			printf("ERROR::INIT: Failed to init glfw\n");
			exit(-1);
		}

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		GLFWwindow *window = glfwCreateWindow(600, 600, "Nap_Nap Tic-Tac-Toe", NULL, NULL);
		if (!window) {
			glfwTerminate();
			printf("ERROR::INIT: Failed to create window\n");
			exit(-1);
		}
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwTerminate();
			printf("ERROR::INIT: Failed to get proc address\n");
			exit(-1);
		}

		ResourceManager::LoadShader("line.vs", "line.frag", "line");
		// ResourceManager::LoadShader("tile.vs", "tile.frag", "tile");
		ResourceManager::LoadShader("piece.vs", "piece.frag", "piece");

		ResourceManager::LoadTexture("X.png", true, "X");
		ResourceManager::LoadTexture("O.png", true, "O");

		ResourceManager::LoadTexture("h_line.png", true, "horizontal");
		ResourceManager::LoadTexture("v_line.png", true, "vertical");
		ResourceManager::LoadTexture("d_r_line.png", true, "diagonal_r");
		ResourceManager::LoadTexture("d_l_line.png", true, "diagonal_l");

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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	game.render();

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

	char *state = game.board.GetTilesState();
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
