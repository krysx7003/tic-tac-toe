#pragma once

#include <sys/types.h>

class TileState {
  public:
	static const u_int Empty = 0;
	static const u_int TakenX = 1;
	static const u_int TakenO = 2;
};
