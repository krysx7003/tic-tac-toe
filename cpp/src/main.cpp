#include <cstdio>
#include <cstdlib>

#include "thirdparty/glad/glad.h"
#include "thirdparty/imgui/backends/imgui_impl_glfw.h"
#include "thirdparty/imgui/backends/imgui_impl_opengl3.h"
#include "thirdparty/imgui/imgui.h"
#include <GLFW/glfw3.h>
#include <string>

#include "game.h"
#include "utils/button.h"
#include "utils/menu.h"

using json = nlohmann::json;
using namespace std;

GLFWwindow *window;

Game game;
Menu main_menu;
Menu top_menu;

int text_id;

bool lock_visible = true;
bool debug_visible;

Text_Field *main_text;
Text_Field *sub_text;
Text_Field *player_text;
Button *start_btn;
Button *restart_btn = nullptr;
Button *exit_btn;
Button *menu_btn;

json config;
double currX, currY;
int click_count = 0;
int top_menu_h = 0;

GLFWwindow *init();
void render(GLFWwindow *window);
void render_debug_frame(GLFWwindow *window);
static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void initMainMenu();
void initPauseMenu();
void initEndGameMenu();
void initTopMenu();
bool isValidInput(char c);

void exit();
void start();
void menu();
void restart();
void resume();

int main() {
	config = ResourceManager::LoadConfig();
	bool gui = config["gui"];
	top_menu_h = config["top_menu"]["height"];
	debug_visible = config["debug_visible"];

	if (gui) {
		window = init();
		game.Init();

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

void initMainMenu() {
	main_menu.SetName("main");
	main_menu.SetBgColor("#282A2E");
	main_menu.SetFgColor("#C5C8C6");
	main_menu.SetOutColor("#0a0a0b");
	main_menu.SetOutline(glm::bvec4(true));

	main_text = dynamic_cast<Text_Field *>(
		main_menu.AddItem(Gui_Item::Type::TEXT_FIELD, 250, 60, "Tic-Tac-Toe", false));
	main_text->SetTextSize("big");
	main_text->SetAlignmentHor(AlignmentHor::CENTER);

	sub_text = dynamic_cast<Text_Field *>(
		main_menu.AddItem(Gui_Item::Type::TEXT_FIELD, 150, 30, "by Nap_Nap", false));
	sub_text->SetPadding(0);
	sub_text->SetTextSize("small");

	start_btn =
		dynamic_cast<Button *>(main_menu.AddItem(Gui_Item::Type::BUTTON, 225, 50, "Start", false));
	start_btn->button_text.SetTextSize("medium");
	start_btn->SetOnClick(start);

	exit_btn = dynamic_cast<Button *>(main_menu.AddItem(Gui_Item::Type::BUTTON, 225, 50, "Exit"));
	exit_btn->button_text.SetTextSize("medium");
	exit_btn->SetOnClick(exit);
}

void initPauseMenu() {
	if (main_menu.Name == "pause")
		return;

	main_menu.SetName("pause");
	main_text->SetText("Paused");

	if (sub_text != nullptr) {
		main_menu.RemoveItem(sub_text->Menu_id, false);
		sub_text = nullptr;
	}
	start_btn->SetVisibility(true);
	start_btn->button_text.SetText("Resume");
	start_btn->SetOnClick(resume);

	if (restart_btn == nullptr) {
		restart_btn = dynamic_cast<Button *>(
			main_menu.AddItem(Gui_Item::Type::BUTTON, 225, 50, "Restart", true, 2));
		restart_btn->button_text.SetTextSize("medium");
		restart_btn->SetOnClick(restart);
	}
}

void initEndGameMenu() {
	if (main_menu.Name == "end")
		return;
	initPauseMenu();
	main_menu.SetName("end");

	start_btn->SetVisibility(false);
	main_text->SetText("Game ended");

	sub_text = dynamic_cast<Text_Field *>(
		main_menu.AddItem(Gui_Item::Type::TEXT_FIELD, 150, 40, "Sub", false, 1));

	sub_text->SetPadding(0);
	sub_text->SetTextSize("medium");
	sub_text->SetAlignmentHor(AlignmentHor::CENTER);

	if (game.GetWinner() != '-') {
		sub_text->SetTextf("Winner %c", game.GetWinner());
	} else {
		sub_text->SetText("Draw");
	}

	lock_visible = true;
	main_menu.UpdateItems();
}

void initTopMenu() {
	top_menu.SetBgColor("#282A2E");
	top_menu.SetFgColor("#C5C8C6");
	top_menu.SetOutColor("#0a0a0b");

	top_menu.SetOutline(glm::bvec4(false, false, true, false));
	top_menu.SetOutlineWidth(2.0f);
	top_menu.SetLayout(Menu::Layout::ROW);

	menu_btn =
		dynamic_cast<Button *>(top_menu.AddItem(Gui_Item::Type::BUTTON, 150, 40, "Menu", false));
	menu_btn->SetOutline(glm::bvec4(false, true, false, false));
	menu_btn->SetOnClick(menu);

	player_text = dynamic_cast<Text_Field *>(
		top_menu.AddItem(Gui_Item::Type::TEXT_FIELD, 300, 40, "Current player O"));
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
	glfwSetKeyCallback(window, key_callback);

	main_menu = Menu(window, 300, 480, 150, 80);
	top_menu = Menu(window, 600, 40, 0, 600);

	initMainMenu();
	initTopMenu();

	return window;
}

void render(GLFWwindow *window) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	game.Render();

	if (!game.active) {
		initEndGameMenu();
	}

	if (lock_visible) {
		main_menu.SetVisibility(true);
	}

	main_menu.Draw();

	top_menu.Draw();

	if (debug_visible) {
		render_debug_frame(window);
	}

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
	ypos -= top_menu_h;
	currY = -((ypos / 300) - 1);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
	if (io.WantCaptureMouse)
		return;

	if (main_menu.Visible)
		return;

	if (currY >= 1.0f)
		return;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		game.ChosenTile(currX, currY);
		click_count++;

		player_text->SetTextf("Current player %c", game.GetPlayer());
	}
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (lock_visible)
		return;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		main_menu.SetVisibility(!main_menu.Visible);
	}
}

void exit() { glfwSetWindowShouldClose(window, true); }

void start() {
	lock_visible = false;

	main_menu.SetVisibility(false);
	initPauseMenu();
}

void restart() {
	game.Restart();

	lock_visible = false;
	main_menu.SetVisibility(false);

	initPauseMenu();
}

void menu() {
	if (lock_visible)
		return;

	main_menu.SetVisibility(true);
}

void resume() { main_menu.SetVisibility(false); }
