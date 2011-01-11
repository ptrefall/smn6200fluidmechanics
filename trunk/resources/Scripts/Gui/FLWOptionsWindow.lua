
if(FLWOptionsWindow == nil) then
	FLWOptionsWindow = {}
end

-- For gui events, the content is always the following:
-- event.id -- type of the event. For instance click
-- event.arg0 -- holds the gui document in which the event fired
-- event.arg1 -- holds the gui element context in which the event fired

function FLWOptionsWindow:OnSubmitClicked(event)
	local submit = event.Parameters["submit"]
	if(submit == "ok") then
		local entity = GetSelectedEntity()
		if(entity == nil) then
			return
		end
			
		local alpha = event.Parameters["alphaselect"]
		if(alpha ~= nil) then
			if(alpha == "opaque") then
				entity:SetAlpha(1.0)
			elseif(alpha == "transparent") then
				entity:SetAlpha(0.5)
			elseif(alpha == "hide") then
				entity:SetAlpha(0.0)
			end
		end
		
		local point_spread = event.Parameters["pspread_select"]
		if(point_spread ~= nil) then
			if(point_spread == "small") then
				entity:SetPointSize(2.0)
			elseif(point_spread == "medium") then
				entity:SetPointSize(5.0)
			elseif(point_spread == "large") then
				entity:SetPointSize(9.0)
			end
		end
		
		local add_comp = event.Parameters["add_comp"]
		if(add_comp ~= nil and add_comp ~= "") then
			entity:AddComponent(add_comp)
		end
	end
end
