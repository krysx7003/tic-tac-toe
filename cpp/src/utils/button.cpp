#include "button.h"

void Button::Draw() {
	if (!Visible)
		return;

	this->drawSquare(BgColor);
	this->button_text.Draw();

	if (isMouseOn()) {
		this->drawSquare(glm::vec4(1.0f, 1.0f, 1.0f, 0.25f));

		if (isMouseOn()) {
			this->drawSquare(glm::vec4(1.0f, 1.0f, 1.0f, 0.25f));
			bool isMousePressed =
				(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

			if (isMousePressed && !wasMousePressed) {
				isClickHandled = false;
			}

			if (isMousePressed && !isClickHandled) {
				On_Click();
				isClickHandled = true;
			}

			wasMousePressed = isMousePressed;
		} else {
			wasMousePressed = false;
			isClickHandled = false;
		}
	}

	this->drawOutline();
}
bool Button::isMouseOn() {
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	int y = (window_height * 2) - mouseY;
	int x = mouseX;

	if (start_pos_x > x || x > start_pos_x + width) {
		return false;
	}
	if (start_pos_y > y || y > start_pos_y + height) {
		return false;
	}

	return true;
}

void Button::SetStartY(int start_y) {
	this->button_text.SetStartY(start_y);
	this->start_pos_y = start_y;
	this->setupVertices();
}

void Button::On_Click() { on_clik_func(); }

void Button::SetOnClick(void (*func)()) { this->on_clik_func = func; }
