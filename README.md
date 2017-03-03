# samp-lua
This DLL / ASI installs hooks to invoke lua events and registeres some exports several SA:MP functions to lua.


Similar projects can be found here:
https://github.com/drakeee/samp-plugin-lua
https://github.com/THE-FYP/SAMP.Lua

Other libraries that I used:
- The LUA C API:
https://www.lua.org/pil/24.html
- Sol2:
http://sol2.readthedocs.io/en/latest/

# WIP (Core functionality already written but not uploaded yet): The Lua Engine
What is Lua? Lua is a scripting language, that is designed for being embedded into other application.
You can add a `events.lua` file to your SA:MP directory. The Lua engine emits events when SA:MP functions are called
For example if you call the function SendChat by using your Keybinder or by yourself the engine emits the "onSendChat" event.
This event is handled by triggering the "onSendChat" a lua function inside of the `events.lua` file.
For Example:
```lua
function onSendChat(msg)
-- do something with your message
-- return a true value to mark the command as handled, the command won't be sent.
end
```
Not only your commands emit an event. Almost every SA:MP function calls an event.
For Example (A simple Ad-Blocker):
```lua
function onAddChatMessage(text)
    if string.match(text, 'BUY PREMIUM NOW') then
        return true
    end
    return false
end
```

You can also call the SA:MP functions inside lua:

```lua
function onAddChatMessage(text)
    print(text) -- I repeat every message
    return false
end
```
This way to perform a response is much more faster than using a Keybinder:
```lua
function onAddChatMessage(text)
    if (text == 'Someone wants to refill your car for 1337$') then
        sendCommand('/accept refill')
    end
    return false
end
```

You can register new commands or overwrite some:
```lua
function onSendChat(msg)
	if msg == "/bk" then
		sendCommand('/f I need some backup!')
		return true
	end
	if msg == "/help" then
		sendCommand('/admins')
		return true
	end
end
```

Another example could be:
```lua
function onAddChatMessage(text)
    if string.match(text, 'SMS:') then
        showGameText(text, <iTime>, <iStyle>) -- SMS are important, I need them big!
    end
    return false
end
```
Death event:
```lua
function onDeath()
    sendChat('I just died in your arms tonight');
end
```
Spam Filter:
```lua
local lastmsg
function onAddChatMessage(text)
    
	if lastmsg == text then
		return true
	end
	
	lastmsg = text
	
	return false
end
```

Example for manipulating an AddChatMessage:
```lua
function onAddChatMessage(text)
	if (string.find(text, 'Noch ') and string.find(text, ' Minuten bis zum Payday')) then
		local minutes = string.match(text, 'Noch (%d+) Minuten bis zum Payday')
		full_print('Geldregen in ' .. minutes .. ' Minuten', 2,  'Toller Prefix', 16728128, 5017777)
		return true
	end
end
```
The shortest samp inline calculator ever: Just write `calc: <expr>`, e.g. `calc: 1+3*192/9`
```lua
function onAddChatMessage(text)
	if (string.find(text, 'calc: ')) then
		local result = load('return '.. string.sub(text, 7))()
		print('Result: ' .. result)
		return true
	end
end
```
You may also use the default lua libraries like io, math, os, etc.

I want to implement as much events as possible. If you have suggestions, feel free to use the issue tracker.


The lua script can be reloaded during the game with `/reloadlua`.

## TODO

| Events | Comment | Written |
| ------------- |-------------| -----:|
| onSendChat | handles your chat and your commands | Yes |
| onAddChatMessage | handles the incoming chat messages | Yes |
| onShowGameText |  | Not yet |
| onShowDialog |  | Not yet |
| onDeath |  | Not yet |
| onSpawn |  | Not yet |
| onObjectCreation |  | Not yet |
| onVehicleEnter |  | Not yet |
| onVehicleExit |  | Not yet |
| onVehicleCreation |  | Not yet |
| onFreeze |  | Not yet |
| onUnfreeze |  | Not yet |
| onFreeze |  | Not yet |
| onTeleport |  | Not yet |
| onEngineStart |  | Not yet |
| onEngineStop |  | Not yet |
| ... | | |

| Callbacks | Comment | Written |
| ------------- |-------------| -----:|
| sendChat | sends a chat message | Yes |
| sendCommand | sends a command | Yes |
| addChatMessage | alias: print, shows a client message | Yes |
| full_print | The full version of the print function. Call it like this: full_print(text, chattype, prefix, color, prefixColor). | Yes |

SA:MP uses several ChatTypes that you can invoke using ``:
- 2 for normal chat
- 4 for info messages (blue font color)
- 8 for debug messages (green font color)

If you want to test the API do the following:

- Clone this repository
- Install Boost and Lua, via NuGet Manager (Make sure that you are using all package resources)
- Make sure that the sol2 library is in the root directory (if not, download it manually)
- Compile

To use the engine, inject the lua.dll, then the samp-lua.dll.
Later I you will only need to copy the samp-lua.asi into your SA:MP directory

Keep in mind that you should have a corresponding lua file in your SA:MP directory. At the moment the lua file has to be named `events.lua`.

Have fun implementing some neat things using this engine.

Greets and Have Fun

LuxXx
