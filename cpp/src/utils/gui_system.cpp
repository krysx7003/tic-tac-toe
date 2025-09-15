#include "gui_system.h"

bool Gui_System::inputHandled = false;
Button *Gui_System::focusedButton = nullptr;

bool Gui_System::Handle() {
	if (focusedButton == nullptr)
		return false;

	if (inputHandled)
		return false;

	return focusedButton->Handle();
}

void Gui_System::ResetInputHandled() { inputHandled = false; }

void Gui_System::SetFocus(Button *item) { focusedButton = item; }

Button *Gui_System::GetFocus() { return focusedButton; }
