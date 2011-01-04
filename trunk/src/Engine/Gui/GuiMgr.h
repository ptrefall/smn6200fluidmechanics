#pragma once

namespace Engine
{
	class GuiMgr
	{		
	public:
		GuiMgr(const bool &fullscr, const int &width, const int &height, const int &depth, const int &vsync);
		~GuiMgr();

		bool isWindowOpen() const;
		int getWidth() const;
		int getHeight() const;
		void setCaptionText(const char *text);
		void swapBuffers();
	};
}
