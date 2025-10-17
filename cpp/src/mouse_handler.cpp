#include "mouse_handler.h"
#include "gui/gui_system.h"

Tile *MouseHandler::focusedTile = nullptr;

bool MouseHandler::Handle(bool gui_visible) {
	if (gui_visible) {
		Gui_System::Handle();

	} else if (Gui_System::GetFocus() != nullptr) {
		Gui_System::Handle();

	} else if (focusedTile != nullptr) {
		focusedTile->Handle();
		return true;
	}

	return false;
}
void MouseHandler::SetFocusGui(Button *button) { Gui_System::SetFocus(button); }
void MouseHandler::SetFocusTile(Tile *tile) { focusedTile = tile; }

Tile *MouseHandler::GetFocusTile() { return focusedTile; }
Button *MouseHandler::GetFocusButton() { return Gui_System::GetFocus(); }
