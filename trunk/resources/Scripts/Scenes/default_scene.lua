
Scene = {}

function Scene:Init()
	--local cube = CreateEntity("Cube")
	
	local skybox = CreateEntity("Skybox")
	
	local flw = CreateEntity("Flow3D")
	flw:SetMesh("particles");
	
	--local bucket = CreateEntity("StlMesh", "HalfBucketXYZ")
	--bucket:SetMesh(bucket.name)
	
	--local pipe = CreateEntity("StlMesh", "HalfPipeXYZ")
	--pipe:SetMesh(pipe.name)
	
	local startup_gui = CreateGuiDocument("Main", "startup")
	--local transf_gui = CreateGuiDocument("Main", "transform_menu")
	--local stl_options = CreateGuiDocument("Main", "stl_options")
end

Scene:Init()
