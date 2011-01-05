#pragma once

#include <Core/CoreMgr.h>
#include <Core/ITimer.h>

namespace Engine
{
	class GuiSystemInterface : public Rocket::Core::SystemInterface
	{
	public:
		GuiSystemInterface(CoreMgr *coreMgr) { this->coreMgr = coreMgr; }
		virtual ~GuiSystemInterface() {}
		virtual float GetElapsedTime() { return (float)coreMgr->getTimer()->getTimeElapsed(); }
		virtual bool LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message)
		{
			if(type < Rocket::Core::Log::LT_WARNING)
				throw CL_Exception(message.CString());
			else
				CL_Console::write_line(message.CString());
			
			return true;
		}

	private:
		CoreMgr *coreMgr;
	};

}
