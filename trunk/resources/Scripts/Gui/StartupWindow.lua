
if(StartupWindow == nil) then
	StartupWindow = {}
end

-- For gui events, the content is always the following:
-- event.id -- type of the event. For instance click
-- event.arg0 -- holds the gui document in which the event fired
-- event.arg1 -- holds the gui element context in which the event fired

function StartupWindow:OnNewProjClicked(event)
	local next_window = CreateGuiDocument(event.arg1.name, "new_proj")
	--event.arg0:GetOwnerDocument():Close()
end

function StartupWindow:OnLoadProjClicked(event)
	local next_window = CreateGuiDocument(event.arg1.name, "load_proj")
	--event.arg0:GetOwnerDocument():Close()
end

function StartupWindow:OnExitClicked(event)
end
