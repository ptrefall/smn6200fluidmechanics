
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
	
end

function ContentWindow:OnFileOpenSTLClicked(event)
	
end
