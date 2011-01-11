
if(ContentWindow == nil) then
	ContentWindow = {}
end

-- For gui events, the content is always the following:
-- event.id -- type of the event. For instance click
-- event.arg0 -- holds the gui document in which the event fired
-- event.arg1 -- holds the gui element context in which the event fired

function ContentWindow:OnOpenSTLClicked(event)
	local next_window = CreateGuiDocument(event.arg1.name, "open_stl")
end

function ContentWindow:OnOpenFLWClicked(event)
	local next_window = CreateGuiDocument(event.arg1.name, "open_flw")
end

function ContentWindow:OnFileOpenFLWClicked(event)
	local submit = event.Parameters["submit"]
	if(submit == "open") then
		local flw_name = event.Parameters["flw_name"]
		if(flw_name ~= nil) then
			local flw = CreateEntity("Flow3D", flw_name)
			if(flw ~= nil) then
				flw:SetScale(50.0)
				flw:SetPointSize(5.0)
				flw:SetShaderName(flw_name)
				flw:SetMesh(flw_name)
				AddEntity(flw)
			end
		else
			Print("Failed to open flow3D mesh file!")
		end
	end
end

function ContentWindow:OnFileOpenSTLClicked(event)
	local submit = event.Parameters["submit"]
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
	end
end
