#pragma once

namespace Engine
{
	class CoreMgr;

	class GuiSystemInterface;
	class GuiRenderInterface;

	class GuiMgr
	{		
	public:
		GuiMgr(CoreMgr *coreMgr, const bool &fullscr, const int &width, const int &height, const int &depth, const int &vsync);
		~GuiMgr();

		bool isWindowOpen() const;
		int getWidth() const;
		int getHeight() const;
		void setCaptionText(const char *text);
		void swapBuffers();

		void update(float dt);
		void render();
		void resize(int w, int h);

		void addContext(const CL_String &name, const int &width, const int &height);
		void addDocument(const CL_String &context_name, const CL_String &path);
		void addFont(const CL_String &path);

	private:
		GuiSystemInterface *system;
		GuiRenderInterface *renderer;
		std::vector<Rocket::Core::Context*> contexts;
		std::vector<Rocket::Core::ElementDocument*> documents;

		CL_Mat4f orthoMatrix;
	};
}
