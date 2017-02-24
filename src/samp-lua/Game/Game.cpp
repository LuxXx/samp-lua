#include "Game.hpp"
#include "../../../sol2/sol.hpp"
#include "../Utils/Windows.hpp"

sol::state lua;

void initGame() {

	// initialize SAMP
	{
		while (GetModuleHandleA("samp.dll") == NULL)
			Sleep(50);
	}

}