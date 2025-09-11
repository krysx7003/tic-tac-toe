#include "text_field.h"
#include <cstdio>

void Text_Field::updateTextPos() {
	if (alignment == AlignmentHor::CENTER) {
		text_padding_x = (width - text->TextWidth(Text, size)) / 2;
		return;
	}
	if (alignment == AlignmentHor::RIGHT) {
		printf("ERROR::TEXT_FIELD: This alignment is not yet implemented\n");
	}
}

void Text_Field::Draw() {
	drawSquare();
	text->RenderText(Text, size, start_pos_x + text_padding_x, start_pos_y + 10, scale);
}

void Text_Field::SetText(std::string new_text) { Text = new_text; }
void Text_Field::SetScale(float scale) { this->scale = scale; }
void Text_Field::SetTextSize(std::string text_size) {
	this->size = text_size;
	updateTextPos();
}
void Text_Field::SetAlignmentHor(AlignmentHor alignment) {
	this->alignment = alignment;
	updateTextPos();
}
