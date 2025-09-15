#include "button.h"
#include "gui_system.h"

void Button::Draw() {
	if (!Visible)
		return;

	background.Draw(BgColor);
	this->button_text.Draw();

	if (isMouseOn()) {
		background.Draw(HiColor);
		if (!Gui_System::GetFocus() || Gui_System::GetFocus() != this) {
			Gui_System::SetFocus(this);
		}
	}

	outline.Draw(OutColor, outlines);
}

bool Button::Handle() {
	if (isMouseOn()) {
		background.Draw(HiColor);
		On_Click();
	}
	return false;
}

bool Button::isMouseOn() {
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	int y = (window_height)-mouseY;
	int x = mouseX;

	if (start_pos_x > x || x > start_pos_x + width) {
		return false;
	}
	if (start_pos_y > y || y > start_pos_y + height) {
		return false;
	}

	return true;
}

void Button::SetHiColor(std::string hex_color) { this->HiColor = hexToColor(hex_color); }

void Button::SetStartY(int start_y) {
	this->button_text.SetStartY(start_y);
	this->start_pos_y = start_y;
	this->updatePos();
}

void Button::On_Click() { on_click_func(); }

void Button::SetOnClick(std::function<void()> func) { this->on_click_func = func; }

void Button::SetBgColor(std::string color) {
	BgColor = hexToColor(color);
	button_text.SetBgColor(BgColor);
}
void Button::SetBgColor(glm::vec4 color) {
	this->BgColor = color;
	button_text.SetBgColor(BgColor);
}

void Button::SetFgColor(std::string color) {
	glm::vec4 rgba = hexToColor(color);
	FgColor = glm::vec3(rgba.r, rgba.g, rgba.b);
	button_text.SetFgColor(FgColor);
}
void Button::SetFgColor(glm::vec3 color) {
	this->FgColor = color;
	button_text.SetFgColor(FgColor);
}
void Button::SetText(std::string text) { this->button_text.SetText(text); }
std::string Button::GetText() { return this->button_text.GetText(); }
