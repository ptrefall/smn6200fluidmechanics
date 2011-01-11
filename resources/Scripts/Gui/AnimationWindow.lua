
if(AnimationWindow == nil) then
	AnimationWindow = {}
end

-- For gui events, the content is always the following:
-- event.id -- type of the event. For instance click
-- event.arg0 -- holds the gui document in which the event fired
-- event.arg1 -- holds the gui element context in which the event fired

function AnimationWindow:OnPlayClicked(event)
	PlayProjectAnimation()
end

function AnimationWindow:OnStopClicked(event)
	StopProjectAnimation()
end

function AnimationWindow:OnRewindClicked(event)
	RewindProjectAnimation()
end

function AnimationWindow:OnForwardClicked(event)
	ForwardProjectAnimation()
end
