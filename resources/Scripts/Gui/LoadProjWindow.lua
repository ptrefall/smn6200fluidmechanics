
if(LoadProjWindow == nil) then
	LoadProjWindow = {}
end

-- For gui events, the content is always the following:
-- event.id -- type of the event. For instance click
-- event.arg0 -- holds the gui document in which the event fired
-- event.arg1 -- holds the gui element context in which the event fired

function LoadProjWindow:OnLoadClicked(event)
	local submit = event.Parameters["submit"]
	if(submit == "load") then
		local proj_name = event.Parameters["proj_name"]
		if(proj_name ~= nil) then
			local success = LoadProject(proj_name)
			if(success == true) then
				CreateGuiDocument(event.arg1.name, "main")
			end
		else
			Print("Failed to load project file!")
			CreateGuiDocument(event.arg1.name, "startup")
		end
	else
		CreateGuiDocument(event.arg1.name, "startup")
	end
end
