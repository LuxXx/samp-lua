-- This is an example file that contains the prototypes of Lua events.
-------------------------------------
-- Common SAMP events --
-------------------------------------

function onSendChat(msg)
	-- do something
    -- return a true value to mark the command as handled, the command won't be sent.
end

function onShowGameText(text, iTime, iStyle)
	-- do something
	-- return a true value to mark the command as handled, the command won't be sent.
end

function onAddChatMessage(text)
	-- do something
	-- return a true value to mark the command as handled, the command won't be sent.
end

function onShowDialog(id, style, caption, text, button, button2, isServerDialog)
	-- do something
	-- return a true value to mark the command as handled, the command won't be sent.
end

-- and more

-------------------------------------
-- Common GTA events --
-- I do not know if I will implement all of these, because they can be abused quite easily.
-------------------------------------

function onSetPlayerPos(x, y, z)

end

function onPlaySound(sound, x, y, z)

end

-- and more