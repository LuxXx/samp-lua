#include "Game.hpp"
#include "../../../sol2/sol.hpp"
#include "../Utils/Windows.hpp"
#include "../Utils/Module.hpp"

sol::state lua;

DWORD g_dwModuleLength = 0;
DWORD g_dwModuleBase = 0;

void initGame() {

	// initialize SAMP
	{
		while (GetModuleHandleA("samp.dll") == NULL)
			Sleep(50);
	}

	g_dwModuleBase = Utils::Module::moduleBase("samp.dll");
	g_dwModuleLength = Utils::Module::moduleLength((HMODULE)g_dwModuleBase);

	if (g_dwModuleBase == 0 || g_dwModuleLength == 0)
		throw std::exception("Error while initializing SA:MP");

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