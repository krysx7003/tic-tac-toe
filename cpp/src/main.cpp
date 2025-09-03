#include <cstdio>
#include <cstdlib>

#include "thirdparty/glad/glad.h"
#include "thirdparty/imgui/backends/imgui_impl_glfw.h"
#include "thirdparty/imgui/backends/imgui_impl_opengl3.h"
#include "thirdparty/imgui/imgui.h"
#include <GLFW/glfw3.h>
#include <string>

#include "game.h"
#include "utils/menu.h"

using json = nlohmann::json;
using namespace std;

Game game;
Menu main_menu;
Menu top_menu;

json config;
double currX, currY;
int click_count = 0;
int top_menu_h = 0;

GLFWwindow *init();
void render(GLFWwindow *window);
void render_debug_frame(GLFWwindow *window);
static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
bool isValidInput(char c);

int main() {
	config = ResourceManager::LoadConfig();
	bool gui = config["gui"];
	top_menu_h = config["top_menu"]["height"];

	if (gui) {
		GLFWwindow *window = init();
		game.Init();
		main_menu = Menu(300, 480, 150, 80);

		main_menu.AddItem(Gui_Item::Type::TEXT_FIELD, 250, 150, "Main", false);
		main_menu.AddItem(Gui_Item::Type::BUTTON, 225, 50, "Start", false);
		main_menu.AddItem(Gui_Item::Type::BUTTON, 225, 50, "Restart", false);
		main_menu.AddItem(Gui_Item::Type::BUTTON, 225, 50, "Exit");

		top_menu = Menu(600, 40, 0, 600);
		top_menu.SetLayout(Menu::Layout::ROW);
		top_menu.AddItem(Gui_Item::Type::BUTTON, 150, 40, "Menu", false);
		top_menu.AddItem(Gui_Item::Type::TEXT_FIELD, 300, 40, "Next player %d");

		while (!glfwWindowShouldClose(window)) {
			render(window);
		}
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(window);
		glfwTerminate();
	} else {
		game.Init();
		while (true) {
			game.Start();

			char c;
			printf("Restart (y/n): ");
			scanf(" %c", &c);

			while (!isValidInput(c)) {
				printf("Invalid input. Restart (y/n): ");
				scanf(" %c", &c);
			}

			if (c == 'y')
				game.Restart();
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

GLFWwindow *init() {
	if (!glfwInit()) {
		printf("ERROR::INIT: Failed to init glfw\n");
		exit(-1);
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	std::string name =
		config["window"]["i3-prefix"].get<std::string>() + config["name"].get<std::string>();

	GLFWwindow *window = glfwCreateWindow(config["window"]["width"], config["window"]["height"],
										  name.c_str(), NULL, NULL);

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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	return window;
}

void render(GLFWwindow *window) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	game.Render();
	main_menu.Draw();
	top_menu.Draw();

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
	ImGui::Text("Clicked tile: %d", game.GetLastTile());
	ImGui::Text("Current player: %c", game.GetPlayer());

	char *state = game.board.GetTilesState();
	ImGui::Text("| %c | %c | %c |\n-------------", state[0], state[1], state[2]);
	ImGui::Text("| %c | %c | %c |\n-------------", state[3], state[4], state[5]);
	ImGui::Text("| %c | %c | %c |\n-------------", state[6], state[7], state[8]);

	if (!game.active) {
		ImGui::Text("Game ended winner: %c", game.GetWinner());
	}

	if (ImGui::Button("Restart")) {
		game.Restart();
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
	if (ypos >= top_menu_h) {
		ypos -= top_menu_h;
		currY = -((ypos / 300) - 1);
	}
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
	if (io.WantCaptureMouse)
		return;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		click_count++;
		game.ChosenTile(currX, currY);
	}
}
