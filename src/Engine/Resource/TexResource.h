#pragma once

namespace Engine
{

class TexResource
{
public:
	static void beginTextureCube(unsigned int &texture);
	static int createTextureCube(const char *filename, unsigned int side);
	static void endTextureCube(unsigned int &texture);
};

}
