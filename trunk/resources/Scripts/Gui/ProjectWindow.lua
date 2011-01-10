
if(ProjectWindow == nil) then
	ProjectWindow = {}
end

-- For gui events, the content is always the following:
-- event.id -- type of the event. For instance click
-- event.arg0 -- holds the gui document in which the event fired
-- event.arg1 -- holds the gui element context in which the event fired

function ProjectWindow:OnEditClicked(entity, event)
	-- Only bother selecting if entity is of a type we know how to handle...
	if(entity.type == "StlMesh") then
		SelectEntity(entity)
		CreateGuiDocument(event.arg1.name, "stl_options")
	elseif(entity.type == "Flow3D") then
		SelectEntity(entity)
		CreateGuiDocument(event.arg1.name, "flw_options")
	end
end
