
#include <ClanLib/src/API/core.h>
#include "Engine/Core/CoreMgr.h"

int main(int argc, char *argv[])
{
	try
	{
		CL_String path = argv[0];
		path = path.substr(0, path.find_last_of("\\"));
		path = path.substr(0, path.find_last_of("\\"));
		path += "\\resources\\";

		Engine::CoreMgr core(path);
		core.init(path);
		core.run();
	}
	catch(CL_Exception &e)
	{
		CL_Console::write_line(e.get_message_and_stack_trace());
		system("pause");
		return 1;
	}

	system("pause");
	return 0;
}
