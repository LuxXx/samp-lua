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

	initLua();

	while (true) { Sleep(100); }
}

void initLua() {

	lua.open_libraries();

	sol::load_result events_lua = lua.load_file("events.lua");

	if (!events_lua.valid()) {
		// Could not load the events.lua file
		return;
	}

	events_lua();



}