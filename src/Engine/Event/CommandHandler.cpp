#include "precomp.h"
#include "CommandHandler.h"

using namespace Engine;
using namespace Events;

CommandHandler::CommandHandler()
{
}

void CommandHandler::registerKeyCommand(unsigned char key, bool keyDown, const char *command_type)
{
    keyCommands[std::pair<unsigned char, bool>(key, keyDown)] = command_type;
}

const char *CommandHandler::getKeyCommand(unsigned char key, bool keyDown)
{
    std::map<std::pair<unsigned char, bool>, const char *>::iterator it = keyCommands.find(std::pair<unsigned char, bool>(key, keyDown));
    if(it == keyCommands.end())
    {
        return "";
    }
    else
    {
        return it->second;
    }
}

void CommandHandler::registerMouseCommand(int button, int state, const char *command_type)
{
	mouseCommands[std::pair<int, int>(button, state)] = command_type;
}

const char *CommandHandler::getMouseCommand(int button, int state)
{
	std::map<std::pair<int, int>, const char *>::iterator it = mouseCommands.find(std::pair<int, int>(button, state));
    if(it == mouseCommands.end())
    {
        return "";
    }
    else
    {
        return it->second;
    }
}
