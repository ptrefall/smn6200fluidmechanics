
if(ProjectWindow == nil) then
	ProjectWindow = {}
end

-- For gui events, the content is always the following:
-- event.id -- type of the event. For instance click
-- event.arg0 -- holds the gui document in which the event fired
-- event.arg1 -- holds the gui element context in which the event fired

function ProjectWindow:OnEditClicked(entity, event)
	--[[local submit = event.Parameters["submit"]
	if(submit == "open") then
		local stl_name = event.Parameters["stl_name"]
		if(stl_name ~= nil) then
			local mesh = CreateEntity("StlMesh", stl_name)
			if(mesh ~= nil) then
				mesh:SetMesh(mesh.name)
				AddEntity(mesh)
			end
		else
			Print("Failed to open stl mesh file!")
		end
	end--]]
end
