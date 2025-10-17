#pragma once

#include "gui/widgets/button.h"
#include "tile.h"

class MouseHandler {
	static Tile *focusedTile;

  public:
	static bool Handle(bool gui_visible);

	static void SetFocusGui(Button *button);
	static void SetFocusTile(Tile *tile);

	static Tile *GetFocusTile();
	static Button *GetFocusButton();
};
