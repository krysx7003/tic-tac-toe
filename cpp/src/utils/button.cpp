#include "button.h"

void Button::Draw() {
	this->drawSquare();
	this->button_text.Draw();
}

void Button::SetStartY(int start_y) {
	this->button_text.SetStartY(start_y);
	this->start_pos_y = start_y;
	this->setupVertices();
}

void Button::On_Click() {}
