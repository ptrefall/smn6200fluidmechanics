#pragma once

namespace Engine
{
	class CoreMgr;

	class GuiSystemInterface;
	class GuiRenderInterface;
	class GuiEventManager;
	class GuiEventInstancer;

	class GuiMgr //: public Rocket::Core::EventListener
	{		
	public:
		GuiMgr(CoreMgr *coreMgr, const bool &fullscr, const int &width, const int &height, const int &depth, const int &vsync, const bool &debug);
		~GuiMgr();

		bool isWindowOpen() const;
		int getWidth() const;
		int getHeight() const;
		void setCaptionText(const char *text);
		void swapBuffers();

		void update(float dt);
		void render();
		void resize(int w, int h);

		GuiEventManager *getGuiEventMgr() const { return eventMgr; }

		Rocket::Core::Context *addContext(const CL_String &name, const int &width, const int &height);
		Rocket::Core::Context *getContext(unsigned int i) { return contexts[i]; }
		Rocket::Core::ElementDocument *addDocument(const CL_String &context_name, const CL_String &path);
		void addFont(const CL_String &path);
		Rocket::Core::ElementDocument *loadCursor(const CL_String &path);

		void inject(const unsigned int &key, bool state);
		void inject(const CL_Vec2i &mouse_pos);
		void injectMouse(const int &button, bool state);
		void keyModifier(const int &mod, bool state);

		// Process the incoming event.
		//virtual void ProcessEvent(Rocket::Core::Event& event);

	private:
		CoreMgr *coreMgr;
		GuiSystemInterface *system;
		GuiRenderInterface *renderer;
		GuiEventManager *eventMgr;
		GuiEventInstancer *eventInstancer;
		std::vector<Rocket::Core::Context*> contexts;
		std::vector<Rocket::Core::ElementDocument*> documents;

		CL_Mat4f orthoMatrix;

		CL_Vec2i mousePos, lastMousePos;
		int key_modifier_state;
	};
}
