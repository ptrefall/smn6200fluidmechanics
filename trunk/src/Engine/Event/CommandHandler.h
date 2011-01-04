#pragma once

#include <map>

namespace Engine
{
namespace Events
{

class CommandHandler
{
public:
	CommandHandler();

    void registerKeyCommand(unsigned char key, bool keyDown, const char *command_type);
    const char *getKeyCommand(unsigned char key, bool keyDown);

	void registerMouseCommand(int button, int state, const char *command_type);
	const char *getMouseCommand(int button, int state);

private:
    std::map<std::pair<unsigned char, bool>, const char *> keyCommands;
	std::map<std::pair<int, int>, const char *> mouseCommands;
};

}}
