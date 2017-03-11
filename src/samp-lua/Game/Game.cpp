#include "Game.hpp"
#include "../../../sol2/sol.hpp"
#include "../Utils/Windows.hpp"
#include "../Utils/Module.hpp"
#include "../Utils/Pattern.hpp"
#include "../Game/SAMP/PatternTable.hpp"
#include "../Utils/Hook.hpp"

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

	hookAddChatMessage();
	hookSendChatCommand();
	hookSendChat();
}

DWORD *g_dwChat = 0;
typedef void(__thiscall* tAddChatMessage)(DWORD *_this, int type, const char *szString, const char *szPrefix, DWORD textColor, DWORD prefixColor);
tAddChatMessage oAddChatMessage;

void __fastcall hAddChatMessage(DWORD *_this, void *Unknown, int type, const char *szString, const char *szPrefix, DWORD textColor, DWORD prefixColor)
{

	if (g_dwChat == 0) g_dwChat = _this; // TODO: retrieve this from somewhere else

	sol::protected_function onAddChatMessage = lua["onAddChatMessage"];
	onAddChatMessage.error_handler = lua["handler"];

	sol::protected_function_result result = onAddChatMessage(szString, szPrefix);
	if (result.valid()) {
		if (result) {
			return;
		}

		oAddChatMessage(_this, type, szString, szPrefix, textColor, prefixColor);
	}
	else {
		sol::error err = result;
		std::string what = err.what();
		oAddChatMessage(g_dwChat, 2, what.c_str(), std::string("LuxLua-Engine").c_str(), 0xB746464, 0xB30000);
		return;
	}
}

void hookAddChatMessage() {
	static auto addr = Utils::Pattern::findPattern(
		g_dwModuleBase,
		g_dwModuleLength,
		Game::SAMP::PatternTable::FullAddChatMessage::byteMask,
		Game::SAMP::PatternTable::FullAddChatMessage::useMask
	);

	oAddChatMessage = (tAddChatMessage)DetourFunction((PBYTE)addr, (PBYTE)hAddChatMessage);

	auto addChatMessage = [](std::string s) {
		oAddChatMessage(g_dwChat, 8, s.c_str(), std::string("").c_str(), -1, 0);
	};
	lua.set_function("print", addChatMessage);
	lua.set_function("addChatMessage", addChatMessage);

	lua.set_function("full_print", [](std::string s, int type, std::string prefix, DWORD color, DWORD prefixColor) {
		oAddChatMessage(g_dwChat, type, s.c_str(), std::string(prefix).c_str(), color, prefixColor);
	});

}

Utils::Hook::Hook<Utils::Hook::CallConvention::stdcall_t, bool, const char *> g_sendChatCommand;

void hookSendChatCommand() {
	static auto addr = Utils::Pattern::findPattern(
		g_dwModuleBase,
		g_dwModuleLength,
		Game::SAMP::PatternTable::SendChat::Command::byteMask,
		Game::SAMP::PatternTable::SendChat::Command::useMask
	);

	auto sendChatCommand_det = [](const char *msg) {

		if (std::string(msg) == "/reloadlua") {
			sol::load_result events_lua = lua.load_file("events.lua");

			if (events_lua.valid()) {
				events_lua();
				oAddChatMessage(g_dwChat, 8, "events.lua reloaded", std::string("LuxLua-Engine").c_str(), 0xB746464, 0xB30000);
				return false;
			}
			else {
				g_sendChatCommand.remove();
				oAddChatMessage(g_dwChat, 8, "Error reloading events.lua - Hook got removed", std::string("LuxLua-Engine").c_str(), 0xB746464, 0xB30000);
				return false;
			}
		}

		sol::protected_function onSendChat = lua["onSendChat"];
		onSendChat.error_handler = lua["handler"];

		sol::protected_function_result result = onSendChat(msg);
		if (result.valid()) {
			if (result) {
				return false;
			}

			return g_sendChatCommand.callOrig(msg);
		}
		else {
			sol::error err = result;
			std::string what = err.what();
			oAddChatMessage(g_dwChat, 8, what.c_str(), std::string("LuxLua-Engine").c_str(), 0xB746464, 0xB30000);
			return false;
		}
	};

	g_sendChatCommand.apply(addr, sendChatCommand_det);
	lua.set_function("sendCommand", [](std::string s) { return g_sendChatCommand.callOrig(s.c_str()); });

}

Utils::Hook::Hook<Utils::Hook::CallConvention::stdcall_t, bool, const char *> g_sendChat;

void hookSendChat() {

	static auto addr = Utils::Pattern::findPattern(
		g_dwModuleBase,
		g_dwModuleLength,
		Game::SAMP::PatternTable::SendChat::Chat::byteMask,
		Game::SAMP::PatternTable::SendChat::Chat::useMask
		);
	auto sendChat_det = [](const char *msg) {

		sol::protected_function onSendChat = lua["onSendChat"];
		onSendChat.error_handler = lua["handler"];

		sol::protected_function_result result = onSendChat(msg);
		if (result.valid()) {
			if (result) {
				return false;
			}

			return g_sendChat.callOrig(msg);
		}
		else {
			sol::error err = result;
			std::string what = err.what();
			oAddChatMessage(g_dwChat, 8, what.c_str(), std::string("LuxLua-Engine").c_str(), 0xB746464, 0xB30000);
			return false;
		}
	};

	g_sendChat.apply(addr, sendChat_det);
	lua.set_function("sendChat", [](std::string s) { return g_sendChat.callOrig(s.c_str()); });
}