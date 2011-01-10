
Scene = {}

function Scene:Init()
	--local cube = CreateEntity("Cube")
	
	local skybox = CreateEntity("Skybox")
	
	--local bucket = CreateEntity("StlMesh", "HalfBucketXYZ")
	--bucket:SetMesh(bucket.name)
	
	--local pipe = CreateEntity("StlMesh", "HalfPipeXYZ")
	--pipe:SetMesh(pipe.name)
	
	local startup_gui = CreateGuiDocument("Main", "startup")
	--local transf_gui = CreateGuiDocument("Main", "transform_menu")
end

Scene:Init()
