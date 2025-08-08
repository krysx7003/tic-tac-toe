#include <sstream>
#include <string>

#include "include/glad/glad.h"
#include "include/imgui/backends/imgui_impl_glfw.h"
#include "include/imgui/backends/imgui_impl_opengl3.h"
#include "include/imgui/imgui.h"
#include <GLFW/glfw3.h>

#include "board.h"

using namespace std;

const char *vertexShaderSource = "#version 330 core\n"
								 "layout (location = 0) in vec3 aPos;\n"
								 "void main()\n"
								 "{\n"
								 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
								 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
								   "out vec4 FragColor;\n"
								   "uniform vec3 ourColor;\n"
								   "void main()\n"
								   "{\n"
								   "   FragColor = vec4(ourColor, 1.0f);\n"
								   "}\n\0";

GLuint init_shader();
Board board = Board();

void render(GLFWwindow *window, GLuint shaderProgram);
void render_debug_frame(GLFWwindow *window);

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

		GLuint shaderProgram = init_shader();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 130");

		board.setupBuffers();

		while (!glfwWindowShouldClose(window)) {
			render(window, shaderProgram);
			glfwSetCursorPosCallback(window, cursor_position_callback);
			glfwSetMouseButtonCallback(window, mouse_button_callback);
		}
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glDeleteProgram(shaderProgram);
		glfwDestroyWindow(window);

		glfwTerminate();
	}
	return 0;
}

void render(GLFWwindow *window, GLuint shaderProgram) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	board.render(shaderProgram);

	render_debug_frame(window);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void render_debug_frame(GLFWwindow *window) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Debug");
	if (ImGui::Button("Close")) {
		glfwSetWindowShouldClose(window, true);
	}

	ImGui::Text(board.getSize());

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GLuint init_shader() {
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}
