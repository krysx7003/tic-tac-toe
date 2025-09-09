#include "text_field.h"
#include <cstdio>

void Text_Field::Draw() {

	this->drawSquare();
	text->RenderText(this->Text, this->start_pos_x + 10,
					 this->start_pos_y + this->text_offset_y + 10, this->scale);
}

void Text_Field::SetText(std::string new_text) { this->Text = new_text; }
void Text_Field::SetScale(float scale) { this->scale = scale; }
