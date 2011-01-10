#include "precomp.h"
#include "TexResource.h"

using namespace Engine;

void TexResource::beginTextureCube(unsigned int &texture)
{
	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}

int TexResource::createTextureCube(const char *filename, unsigned int side)
{
	GLFWimage *img = new GLFWimage();
	glfwReadImage(filename, img, 0);

    glTexImage2D(side,
                 0,
                 4,
                 img->Width,
                 img->Height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 img->Data);
	glfwFreeImage(img);
    return 0;
}

void TexResource::endTextureCube(unsigned int &texture)
{
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glDisable(GL_TEXTURE_CUBE_MAP);
}
