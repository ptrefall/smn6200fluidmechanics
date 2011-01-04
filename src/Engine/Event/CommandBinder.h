#pragma once

#include "CommandHandler.h"

namespace Engine
{
namespace Events
{

class CommandBinder
{
public:
	static void Bind(CommandHandler *handler)
	{
		/*handler->registerKeyCommand('w', true, CommandHandler::COMMAND_MOVE_FORWARD);
		handler->registerKeyCommand('w', false, CommandHandler::COMMAND_MOVE_FORWARD_END);
		handler->registerKeyCommand('s', true, CommandHandler::COMMAND_MOVE_BACKWARD);
		handler->registerKeyCommand('s', false, CommandHandler::COMMAND_MOVE_BACKWARD_END);
		handler->registerKeyCommand('a', true, CommandHandler::COMMAND_MOVE_LEFT);
		handler->registerKeyCommand('a', false, CommandHandler::COMMAND_MOVE_LEFT_END);
		handler->registerKeyCommand('d', true, CommandHandler::COMMAND_MOVE_RIGHT);
		handler->registerKeyCommand('d', false, CommandHandler::COMMAND_MOVE_RIGHT_END);
		handler->registerKeyCommand('z', true, CommandHandler::COMMAND_ZOOM_IN);
		handler->registerKeyCommand('z', false, CommandHandler::COMMAND_ZOOM_IN_END);
		handler->registerKeyCommand('x', true, CommandHandler::COMMAND_ZOOM_OUT);
		handler->registerKeyCommand('x', false, CommandHandler::COMMAND_ZOOM_OUT_END);

		handler->registerMouseCommand(GLUT_LEFT_BUTTON, GLUT_UP, CommandHandler::COMMAND_SPAWN_TOWER);*/
		//handler->registerMouseCommand(GLUT_RIGHT_BUTTON, GLUT_DOWN, CommandHandler::COMMAND_SPAWN_MINION);
	}
};

}}
