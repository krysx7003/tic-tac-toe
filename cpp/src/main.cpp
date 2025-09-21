#include <cstdio>
#include <cstdlib>

#include "player_manager.h"
#include "thirdparty/glad/glad.h"
#include "thirdparty/imgui/backends/imgui_impl_glfw.h"
#include "thirdparty/imgui/backends/imgui_impl_opengl3.h"
#include "thirdparty/imgui/imgui.h"
#include <GLFW/glfw3.h>
#include <pthread.h>
#include <signal.h>
#include <string>

#include "game.h"
#include "utils/button.h"
#include "utils/dropdown.h"
#include "utils/gui_system.h"
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
Text_Field *player1_text, *player2_text;
Dropdown *player1_drop, *player2_drop;

json config;
double currX, currY;
double spf = 0;
int top_menu_h = 0;
bool players_notified = false;

GLFWwindow *init();
void render(GLFWwindow *window);
void render_debug_frame(GLFWwindow *window);
static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
std::string setPlayer(char player);
bool getInput(std::string option);
void signalHandler(int signum);
void *playerMove(void *arg);

void initMainMenu();
void initPauseMenu();
void initEndGameMenu();
void initTopMenu();

void close_window();
void start();
void menu();
void restart();
void resume();

int main() {
	config = ResourceManager::LoadConfig();
	bool gui = config["gui"];
	top_menu_h = config["top_menu"]["height"];
	debug_visible = config["debug_visible"];
	pthread_t thread;

	if (gui) {
		window = init();
		game.Init();

		double lastTime = glfwGetTime();
		int nbFrames = 0;
		while (!glfwWindowShouldClose(window)) {
			double currentTime = glfwGetTime();
			nbFrames++;
			if (currentTime - lastTime >= 1.0) {
				spf = 1000.0 / double(nbFrames);
				nbFrames = 0;
				lastTime += 1.0;
			}
			render(window);

			if (!PlayerManager::CurrPlayerHuman()) {
				if (!PlayerManager::RequestHandled) {
					PlayerManager::RequestHandled = true;
					pthread_create(&thread, NULL, playerMove, NULL);

				} else {
					int result = pthread_tryjoin_np(thread, NULL);
					if (result == 0) {
						PlayerManager::RequestHandled = false;
					}
				}
			}

			if (game.ended && !players_notified && PlayerManager::RequestHandled) {
				players_notified = true;
				pthread_kill(thread, SIGTERM);
				pthread_join(thread, nullptr);

				if (game.GetWinner() != '-') {
					string msg = "Win:";
					PlayerManager::MsgAll(msg.append(1, game.GetWinner()));
				} else {
					PlayerManager::MsgAll("Draw");
				}
			}
		}
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(window);
		glfwTerminate();
	} else {
		std::string X, O;
		game.Init();

		PlayerManager::PrintOptions();
		X = setPlayer('X');
		O = setPlayer('O');

		while (true) {
			game.Start(X, O);

			game.Print();

			if (getInput("Restart (y/n)")) {
				if (getInput("Change players (y/n)")) {
					PlayerManager::PrintOptions();
					X = setPlayer('X');
					O = setPlayer('O');
				}

				game.Restart();
			} else
				break;
		}
	}

	PlayerManager::CleanUp();
	return 0;
}

bool getInput(std::string option) {
	char c;
	printf("%s: ", option.c_str());
	scanf(" %c", &c);

	while (c != 'y' && c != 'n') {
		printf("Invalid input. %s: ", option.c_str());
		scanf(" %c", &c);
	}

	return (c == 'y');
}

void signalHandler(int signum) { pthread_exit(nullptr); }

void *playerMove(void *arg) {
	int id = -1;
	do {
		signal(SIGTERM, signalHandler);
		PlayerManager::BoardState = game.board.GetState();
		id = PlayerManager::MakeMove();

	} while (!game.ChosenTile(id));

	return NULL;
}

std::string setPlayer(char player) {
	char c;

	printf("Chose player %c: ", player);
	scanf(" %c", &c);

	while (!PlayerManager::ValidOption(c)) {
		printf("Invalid input. Chose player %c: ", player);
		scanf(" %c", &c);
	}

	int id = c - '0';
	return PlayerManager::GetOption(id);
}

void initMainMenu() {
	main_menu.SetName("main");
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

	player1_text = dynamic_cast<Text_Field *>(
		main_menu.AddItem(Gui_Item::Type::TEXT_FIELD, 150, 30, "Player O:", false));
	player1_text->SetTextSize("small");
	player1_text->SetPadding(5);
	player1_drop =
		dynamic_cast<Dropdown *>(main_menu.AddItem(Gui_Item::Type::DROPDOWN, 225, 30, "", false));
	player1_drop->AddItems(PlayerManager::GetOptions());
	player1_drop->SetPadding(0);

	player2_text = dynamic_cast<Text_Field *>(
		main_menu.AddItem(Gui_Item::Type::TEXT_FIELD, 150, 30, "Player X:", false));
	player2_text->SetTextSize("small");
	player2_text->SetPadding(5);

	player2_drop =
		dynamic_cast<Dropdown *>(main_menu.AddItem(Gui_Item::Type::DROPDOWN, 225, 30, "", false));
	player2_drop->AddItems(PlayerManager::GetOptions());
	player2_drop->SetPadding(0);

	exit_btn =
		dynamic_cast<Button *>(main_menu.AddItem(Gui_Item::Type::BUTTON, 225, 50, "Exit", true));
	exit_btn->button_text.SetTextSize("medium");
	exit_btn->SetOnClick(close_window);
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

	player1_text->SetVisibility(false);
	player2_text->SetVisibility(false);
	player1_drop->SetVisibility(false);
	player2_drop->SetVisibility(false);

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

	player1_text->SetVisibility(true);
	player2_text->SetVisibility(true);
	player1_drop->SetVisibility(true);
	player2_drop->SetVisibility(true);

	if (game.GetWinner() != '-') {
		sub_text->SetTextf("Winner %c", game.GetWinner());
	} else {
		sub_text->SetText("Draw");
	}

	main_menu.UpdateItems();
	main_menu.SetVisibility(true);
}

void initTopMenu() {
	top_menu.SetOutline(glm::bvec4(false, false, true, false));
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

	if (game.ended) {
		initEndGameMenu();
	}
	main_menu.Draw();

	player_text->SetTextf("Current player %c", PlayerManager::Curr_player);
	top_menu.Draw();

	main_menu.DrawPopups();

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

	ImGui::Text("%f.02 ms/frame\n", spf);

	ImGui::Text("Position x: %.0f, y: %.0f", currX, currY);
	if (Gui_System::GetFocus() != nullptr) {
		ImGui::Text("Button %s", Gui_System::GetFocus()->GetText().c_str());
	}
	ImGui::Text("Clicked tile: %d", game.GetLastTile());
	ImGui::Text("Current player: %c", PlayerManager::Curr_player);

	std::vector<char> state = game.board.GetTilesState();
	ImGui::Text("| %c | %c | %c |\n-------------", state[0], state[1], state[2]);
	ImGui::Text("| %c | %c | %c |\n-------------", state[3], state[4], state[5]);
	ImGui::Text("| %c | %c | %c |\n-------------", state[6], state[7], state[8]);

	if (game.ended) {
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

	currX = xpos;
	currY = ypos;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
	if (io.WantCaptureMouse)
		return;

	if (main_menu.Visible) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			Gui_System::Handle();
		}
		return;

	} else {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			if (currY <= 40.0f) {
				Gui_System::Handle();
				return;
			}

			if (!PlayerManager::CurrPlayerHuman())
				return;

			game.ChosenTile(currX, currY);
		}
	}
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (lock_visible)
		return;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		main_menu.SetVisibility();
	}
}

void close_window() {
	PlayerManager::CleanUp();
	glfwSetWindowShouldClose(window, true);
}

void start() {
	lock_visible = false;

	main_menu.SetVisibility(false);
	player1_drop->SetDropdown(false);
	player2_drop->SetDropdown(false);
	initPauseMenu();
	main_menu.UpdateItems();

	if (!game.active) {
		game.Start(player1_drop->GetSelected(), player2_drop->GetSelected());
	}
}

void restart() {
	game.Restart();

	lock_visible = false;
	main_menu.SetVisibility(false);

	initPauseMenu();
	main_menu.UpdateItems();

	if (!game.active) {
		PlayerManager::RequestHandled = false;
		game.Start(player1_drop->GetSelected(), player2_drop->GetSelected());
		players_notified = false;
	}
}

void menu() {
	if (lock_visible)
		return;

	main_menu.SetVisibility();
}
void resume() { main_menu.SetVisibility(false); }
