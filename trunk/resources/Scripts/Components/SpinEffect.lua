
if(SpinEffect == nil) then
	SpinEffect = {}
end

function SpinEffect:OnInit(entity)
	entity:AddProperty("Yaw", 0.0)
	entity:AddProperty("YawRate", 1.0)
end

function SpinEffect:OnUpdate(entity, dt)
	entity:SetYaw(entity:GetYaw()+entity:GetYawRate()*dt*0.2)
end

RegisterComponent("SpinEffect")
