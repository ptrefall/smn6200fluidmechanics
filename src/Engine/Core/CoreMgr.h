#pragma once

class CL_SetupCore;

namespace Engine
{
	namespace Events { class IEventManager; }
	class GuiMgr;
	class ResMgr;
	class ScriptMgr;
	class EntityManager;
	class IEntity;
	class WorkThreadMgr;
	class ITimer;
	class Cam;

	class CoreMgr
	{
	public:
		CoreMgr(const CL_String &base_path);
		~CoreMgr();

		Events::IEventManager *getEventMgr() const { return eventMgr; }
		GuiMgr *getGuiMgr() const { return guiMgr; }
		ResMgr *getResMgr() const { return resMgr; }
		ScriptMgr *getScriptMgr() const { return scriptMgr; }
		EntityManager *getEntityMgr() const { return entityMgr; }
		WorkThreadMgr *getWorkThreadMgr() const { return workThreadMgr; }
		ITimer *getTimer() const { return timer; }
		Cam *getCam() const { return cam; }

		void exit() { stop = true; }

	private:
		CL_SetupCore *setupCore; // Initializes clanlib core lib when CoreManager is instanciated/constructed

		void init(const CL_String &base_path);
		void run();
		void checkForInput(float dt);

		Events::IEventManager *eventMgr;
		GuiMgr *guiMgr;
		ResMgr *resMgr;
		ScriptMgr *scriptMgr;
		EntityManager *entityMgr;
		WorkThreadMgr *workThreadMgr;
		ITimer *timer;
		Cam *cam;
		bool stop;
	};
}
