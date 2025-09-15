#pragma once

#include "button.h"

class Gui_System {
	static Button *focusedButton;
	static bool inputHandled;

  public:
	static bool Handle();

	static void ResetInputHandled();
	static void SetFocus(Button *button);
	static Button *GetFocus();
};
