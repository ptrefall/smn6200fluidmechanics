#include "precomp.h"
#include "GuiRenderInterface.h"

#include <Core/CoreMgr.h>
#include <Core/Cam.h>

using namespace Engine;

GuiRenderInterface::GuiRenderInterface(CoreMgr *coreMgr, const CL_Mat4f &orthoMat)
: coreMgr(coreMgr), orthoMat(orthoMat)
{
}

GuiRenderInterface::~GuiRenderInterface()
{
}

// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.		
Rocket::Core::CompiledGeometryHandle GuiRenderInterface::CompileGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture)
{
	unsigned int handle = vao.size()+1;
	unsigned int new_vao, new_ibo, new_vbo;

	std::vector<unsigned int> ind;
	std::vector<float> verts;
	std::vector<float> colors;
	std::vector<float> texCoords;

	for(int i = 0; i < num_indices; i++)
	{
		ind.push_back((unsigned int)indices[i]);
	}
	for(int i = 0; i < num_vertices; i++)
	{
		verts.push_back(vertices[i].position.x);
		verts.push_back(vertices[i].position.y);

		colors.push_back(vertices[i].colour.red/255.0f);
		colors.push_back(vertices[i].colour.green/255.0f);
		colors.push_back(vertices[i].colour.blue/255.0f);
		colors.push_back(vertices[i].colour.alpha/255.0f);

		texCoords.push_back(vertices[i].tex_coord.x);
		texCoords.push_back(vertices[i].tex_coord.y);
		bool hit = false;
		if(texture == 1 &&
			((vertices[i].tex_coord.x == 0.0f && vertices[i].tex_coord.y == 0.0f) ||
			(vertices[i].tex_coord.x == 133.0f/512.0f && vertices[i].tex_coord.y == 140.0f/512.0f)))
		{
			hit = true;
		}
	}

	glGenVertexArrays(1, &new_vao);
	glBindVertexArray(new_vao);

	glGenBuffers(1, &new_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*ind.size(), &ind[0], GL_STATIC_DRAW);

	glGenBuffers(1, &new_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, new_vbo);
	glBufferData(GL_ARRAY_BUFFER, (verts.size()+colors.size()+texCoords.size())*sizeof(float), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*verts.size(), &verts[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)*verts.size(), sizeof(float)*colors.size(), &colors[0]);

	if(texture > 0)
	{
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)*(verts.size()+colors.size()), sizeof(float)*texCoords.size(), &texCoords[0]);

		ShaderObj shader;
		shader.setShader("resources/Shaders/gui_default");
		shader.initShader();
		Engine::shaderAttrib(shader.getShaderProg(), "vVertex", 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		Engine::shaderAttrib(shader.getShaderProg(), "vColor", 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(float)*verts.size()));
		Engine::shaderAttrib(shader.getShaderProg(), "vTexCoord", 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(float)*(verts.size()+colors.size())));
		shaders.push_back(shader);
	}
	else
	{
		ShaderObj shader;
		shader.setShader("resources/Shaders/gui_notex");
		shader.initShader();
		Engine::shaderAttrib(shader.getShaderProg(), "vVertex", 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		Engine::shaderAttrib(shader.getShaderProg(), "vColor", 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(float)*verts.size()));
		shaders.push_back(shader);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	vao.push_back(new_vao);
	ibo.push_back(new_ibo);
	vbo.push_back(new_vbo);
	index_size.push_back(ind.size());
	tex_id.push_back(texture);

	return handle;
}

// Called by Rocket when it wants to render application-compiled geometry.		
void GuiRenderInterface::RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	shaders[geometry-1].enableShader();
	{
		bindTexture(geometry-1);
		bindUniforms(geometry-1, translation);
		glBindVertexArray(vao[geometry-1]);
		glDrawElements(GL_TRIANGLES, index_size[geometry-1], GL_UNSIGNED_INT, BUFFER_OFFSET(0));
		glBindVertexArray(0);
		unbindTexture();
	} shaders[geometry-1].disableShader();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_BLEND);
}

// Called by Rocket when it wants to release application-compiled geometry.		
void GuiRenderInterface::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry)
{
	glDeleteBuffers(1, &ibo[geometry-1]);
	glDeleteBuffers(1, &vbo[geometry-1]);
	glDeleteVertexArrays(1, &vao[geometry-1]);
}

void GuiRenderInterface::bindUniforms(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation)
{	
	CL_Mat4f translateMat = CL_Mat4f::identity();
	translateMat[12] = translation.x;
	translateMat[13] = translation.y;
	translateMat[14] = 0.0f;
	
	CL_Mat4f modelMat = CL_Mat4f::identity();
	modelMat = modelMat * CL_Mat3f::identity(); //No rotation
	modelMat = modelMat * translateMat;
	
	CL_Mat4f mvMat = modelMat * orthoMat;

	/*int loc = glGetUniformLocation(shaders[geometry].getShaderProg(), "orthoMat");
	if(loc < 0)
		throw CL_Exception("LOC was -1");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &orthoMat[0]);*/
	
	int loc = glGetUniformLocation(shaders[geometry].getShaderProg(), "mvMat");
	if(loc < 0)
		throw CL_Exception("LOC was -1");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &mvMat[0]);

	if(tex_id[geometry] > 0)
	{
		loc = glGetUniformLocation(shaders[geometry].getShaderProg(), "fTexture");
		if(loc < 0)
			throw CL_Exception("LOC was -1");
		glUniform1i(loc, 0);
	}
}

void GuiRenderInterface::bindTexture(Rocket::Core::CompiledGeometryHandle geometry)
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_id[geometry]);
}

void GuiRenderInterface::unbindTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Called by Rocket when it wants to enable or disable scissoring to clip content.		
void GuiRenderInterface::EnableScissorRegion(bool enable)
{
	if (enable)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);
}

// Called by Rocket when it wants to change the scissor region.		
void GuiRenderInterface::SetScissorRegion(int x, int y, int width, int height)
{
	/*int scissor_left = x;
	int scissor_top = y;
	int scissor_right = x + width;
	int scissor_bottom = y + height;
	glScissor(scissor_left, scissor_top, scissor_right, scissor_bottom);*/
}

// Called by Rocket when a texture is required by the library.		
bool GuiRenderInterface::LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source)
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);

	GLFWimage *img = new GLFWimage();
	glfwReadImage(source.CString(), img, 0);

	texture_dimensions.x = img->Width;
	texture_dimensions.y = img->Height;
	unsigned char *image_dest = img->Data;

	bool success = GenerateTexture(texture_handle, image_dest, texture_dimensions);
	glfwFreeImage(img);

	return success;
}

// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
bool GuiRenderInterface::GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions)
{
	GLuint texture_id = 0;
	glGenTextures(1, &texture_id);
	if (texture_id == 0)
	{
		printf("Failed to generate textures\n");
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, source_dimensions.x, source_dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, source);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	texture_handle = (Rocket::Core::TextureHandle) texture_id;

	return true;
}

// Called by Rocket when a loaded texture is no longer required.		
void GuiRenderInterface::ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
{
	glDeleteTextures(1, (GLuint*) &texture_handle);
}
