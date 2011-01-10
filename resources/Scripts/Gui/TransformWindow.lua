
if(TransformWindow == nil) then
	TransformWindow = {}
end

-- For gui events, the content is always the following:
-- event.id -- type of the event. For instance click
-- event.arg0 -- holds the gui document in which the event fired
-- event.arg1 -- holds the gui element context in which the event fired

function TransformWindow:OnZoomInClicked(event)
	ZoomInCamera(1.0)
end

function TransformWindow:OnZoomOutClicked(event)
	ZoomOutCamera(1.0)
end

function TransformWindow:OnMoveUpClicked(event)
	MoveUpCamera(0.1)
end

function TransformWindow:OnMoveDownClicked(event)
	MoveDownCamera(0.1)
end

function TransformWindow:OnRotateXPosClicked(event)
	PitchCamera(0.01)
end

function TransformWindow:OnRotateXNegClicked(event)
	PitchCamera(-0.01)
end

function TransformWindow:OnRotateYPosClicked(event)
	YawCamera(0.01)
end

function TransformWindow:OnRotateYNegClicked(event)
	YawCamera(-0.01)
end
