
if(STLOptionsWindow == nil) then
	STLOptionsWindow = {}
end

-- For gui events, the content is always the following:
-- event.id -- type of the event. For instance click
-- event.arg0 -- holds the gui document in which the event fired
-- event.arg1 -- holds the gui element context in which the event fired

function STLOptionsWindow:OnCloseClicked(event)
end

function STLOptionsWindow:OnResize(event)
	local value = event.Parameters["alpha_value"]
	if(value ~= nil) then
		Print("Value: " .. value)
	end
end
