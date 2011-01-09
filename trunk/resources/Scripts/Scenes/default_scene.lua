
Scene = {}

function Scene:Init()
	--local cube = CreateEntity("Cube")
	local mesh = CreateEntity("StlMesh")
	mesh:SetMesh("PipeXYZ")
	local startup_gui = CreateGuiDocument("Main", "startup")
end

Scene:Init()
