#include "precomp.h"
#include "Input.h"
#include "InputAsciiMap.h"
#include <Core/CoreMgr.h>
#include <Gui/GuiMgr.h>
#include <windows.h>

using namespace Engine;

//Protect global value in anonymous namespace.
//This way the pointer won't escape this cpp file's context.
//Best we can do to protect this thing from exploding when we
//have to deal with global stuff...
namespace
{
	CoreMgr *coreMgr;

	static const int KEYMAP_SIZE = 256;
	static Rocket::Core::Input::KeyIdentifier key_identifier_map[KEYMAP_SIZE];
}

bool Input::Initialise(CoreMgr *coreManager)
{
	coreMgr = coreManager;
	Input::InitialiseKeymap();
	glfwSetKeyCallback(Input::processKeyboardEvent);
	glfwSetCharCallback(Input::processCharEvent);
	glfwSetMouseButtonCallback(Input::processMouseButtonEvent);
	//glfwSetMousePosCallback(Input::processMouseMoveEvent);
	return true;
}

void Input::processKeyboardEvent(int key, int action)
{
	if(key == GLFW_KEY_ESC)
	{
		coreMgr->exit();
		return;
	}
	Rocket::Core::Input::KeyIdentifier key_identifier = key_identifier_map[key];
	coreMgr->getGuiMgr()->inject(key_identifier, (action == GLFW_PRESS), Input::GetKeyModifierState());
}

void Input::processCharEvent(int key, int action)
{
	Rocket::Core::Input::KeyIdentifier key_identifier = key_identifier_map[(char)key];
	coreMgr->getGuiMgr()->inject(key_identifier, (action == GLFW_PRESS), Input::GetKeyModifierState());
}

void Input::processMouseButtonEvent(int button, int action)
{
	coreMgr->getGuiMgr()->injectMouse(button, (action == GLFW_PRESS), Input::GetKeyModifierState());
}

void Input::processMouseMoveEvent(int x, int y)
{
	coreMgr->getGuiMgr()->inject(CL_Vec2i(x,y), Input::GetKeyModifierState());
}

void Input::InitialiseKeymap()
{
	// Initialise the key map with default values.
	memset(key_identifier_map, 0, sizeof(key_identifier_map));
	
	// Assign individual values.
	key_identifier_map['A'] = Rocket::Core::Input::KI_A;
	key_identifier_map['B'] = Rocket::Core::Input::KI_B;
	key_identifier_map['C'] = Rocket::Core::Input::KI_C;
	key_identifier_map['D'] = Rocket::Core::Input::KI_D;
	key_identifier_map['E'] = Rocket::Core::Input::KI_E;
	key_identifier_map['F'] = Rocket::Core::Input::KI_F;
	key_identifier_map['G'] = Rocket::Core::Input::KI_G;
	key_identifier_map['H'] = Rocket::Core::Input::KI_H;
	key_identifier_map['I'] = Rocket::Core::Input::KI_I;
	key_identifier_map['J'] = Rocket::Core::Input::KI_J;
	key_identifier_map['K'] = Rocket::Core::Input::KI_K;
	key_identifier_map['L'] = Rocket::Core::Input::KI_L;
	key_identifier_map['M'] = Rocket::Core::Input::KI_M;
	key_identifier_map['N'] = Rocket::Core::Input::KI_N;
	key_identifier_map['O'] = Rocket::Core::Input::KI_O;
	key_identifier_map['P'] = Rocket::Core::Input::KI_P;
	key_identifier_map['Q'] = Rocket::Core::Input::KI_Q;
	key_identifier_map['R'] = Rocket::Core::Input::KI_R;
	key_identifier_map['S'] = Rocket::Core::Input::KI_S;
	key_identifier_map['T'] = Rocket::Core::Input::KI_T;
	key_identifier_map['U'] = Rocket::Core::Input::KI_U;	
	key_identifier_map['V'] = Rocket::Core::Input::KI_V;
	key_identifier_map['W'] = Rocket::Core::Input::KI_W;
	key_identifier_map['X'] = Rocket::Core::Input::KI_X;
	key_identifier_map['Y'] = Rocket::Core::Input::KI_Y;
	key_identifier_map['Z'] = Rocket::Core::Input::KI_Z;

	key_identifier_map['0'] = Rocket::Core::Input::KI_0;
	key_identifier_map['1'] = Rocket::Core::Input::KI_1;
	key_identifier_map['2'] = Rocket::Core::Input::KI_2;
	key_identifier_map['3'] = Rocket::Core::Input::KI_3;
	key_identifier_map['4'] = Rocket::Core::Input::KI_4;
	key_identifier_map['5'] = Rocket::Core::Input::KI_5;
	key_identifier_map['6'] = Rocket::Core::Input::KI_6;
	key_identifier_map['7'] = Rocket::Core::Input::KI_7;
	key_identifier_map['8'] = Rocket::Core::Input::KI_8;
	key_identifier_map['9'] = Rocket::Core::Input::KI_9;

	key_identifier_map[GLFW_KEY_BACKSPACE] = Rocket::Core::Input::KI_BACK;
	key_identifier_map[GLFW_KEY_TAB] = Rocket::Core::Input::KI_TAB;

	key_identifier_map[GLFW_KEY_ENTER] = Rocket::Core::Input::KI_RETURN;

	key_identifier_map[GLFW_KEY_PAUSE] = Rocket::Core::Input::KI_PAUSE;
	key_identifier_map[GLFW_KEY_CAPS_LOCK] = Rocket::Core::Input::KI_CAPITAL;

	key_identifier_map[GLFW_KEY_ESC] = Rocket::Core::Input::KI_ESCAPE;

	key_identifier_map[GLFW_KEY_SPACE] = Rocket::Core::Input::KI_SPACE;
	key_identifier_map[GLFW_KEY_END] = Rocket::Core::Input::KI_END;
	key_identifier_map[GLFW_KEY_HOME] = Rocket::Core::Input::KI_HOME;
	key_identifier_map[GLFW_KEY_LEFT] = Rocket::Core::Input::KI_LEFT;
	key_identifier_map[GLFW_KEY_UP] = Rocket::Core::Input::KI_UP;
	key_identifier_map[GLFW_KEY_RIGHT] = Rocket::Core::Input::KI_RIGHT;
	key_identifier_map[GLFW_KEY_DOWN] = Rocket::Core::Input::KI_DOWN;
	key_identifier_map[GLFW_KEY_INSERT] = Rocket::Core::Input::KI_INSERT;
	key_identifier_map[GLFW_KEY_DEL] = Rocket::Core::Input::KI_DELETE;
	
	key_identifier_map[GLFW_KEY_KP_0] = Rocket::Core::Input::KI_NUMPAD0;
	key_identifier_map[GLFW_KEY_KP_1] = Rocket::Core::Input::KI_NUMPAD1;
	key_identifier_map[GLFW_KEY_KP_2] = Rocket::Core::Input::KI_NUMPAD2;
	key_identifier_map[GLFW_KEY_KP_3] = Rocket::Core::Input::KI_NUMPAD3;
	key_identifier_map[GLFW_KEY_KP_4] = Rocket::Core::Input::KI_NUMPAD4;
	key_identifier_map[GLFW_KEY_KP_5] = Rocket::Core::Input::KI_NUMPAD5;
	key_identifier_map[GLFW_KEY_KP_6] = Rocket::Core::Input::KI_NUMPAD6;
	key_identifier_map[GLFW_KEY_KP_7] = Rocket::Core::Input::KI_NUMPAD7;
	key_identifier_map[GLFW_KEY_KP_8] = Rocket::Core::Input::KI_NUMPAD8;
	key_identifier_map[GLFW_KEY_KP_9] = Rocket::Core::Input::KI_NUMPAD9;
	key_identifier_map[GLFW_KEY_KP_MULTIPLY] = Rocket::Core::Input::KI_MULTIPLY;
	key_identifier_map[GLFW_KEY_KP_ADD] = Rocket::Core::Input::KI_ADD;
	key_identifier_map[GLFW_KEY_KP_SUBTRACT] = Rocket::Core::Input::KI_SUBTRACT;
	key_identifier_map[GLFW_KEY_KP_DECIMAL] = Rocket::Core::Input::KI_DECIMAL;
	key_identifier_map[GLFW_KEY_KP_DIVIDE] = Rocket::Core::Input::KI_DIVIDE;
	key_identifier_map[GLFW_KEY_F1] = Rocket::Core::Input::KI_F1;
	key_identifier_map[GLFW_KEY_F2] = Rocket::Core::Input::KI_F2;
	key_identifier_map[GLFW_KEY_F3] = Rocket::Core::Input::KI_F3;
	key_identifier_map[GLFW_KEY_F4] = Rocket::Core::Input::KI_F4;
	key_identifier_map[GLFW_KEY_F5] = Rocket::Core::Input::KI_F5;
	key_identifier_map[GLFW_KEY_F6] = Rocket::Core::Input::KI_F6;
	key_identifier_map[GLFW_KEY_F7] = Rocket::Core::Input::KI_F7;
	key_identifier_map[GLFW_KEY_F8] = Rocket::Core::Input::KI_F8;
	key_identifier_map[GLFW_KEY_F9] = Rocket::Core::Input::KI_F9;
	key_identifier_map[GLFW_KEY_F10] = Rocket::Core::Input::KI_F10;
	key_identifier_map[GLFW_KEY_F11] = Rocket::Core::Input::KI_F11;
	key_identifier_map[GLFW_KEY_F12] = Rocket::Core::Input::KI_F12;
	key_identifier_map[GLFW_KEY_F13] = Rocket::Core::Input::KI_F13;
	key_identifier_map[GLFW_KEY_F14] = Rocket::Core::Input::KI_F14;
	key_identifier_map[GLFW_KEY_F15] = Rocket::Core::Input::KI_F15;
	key_identifier_map[GLFW_KEY_F16] = Rocket::Core::Input::KI_F16;
	key_identifier_map[GLFW_KEY_F17] = Rocket::Core::Input::KI_F17;
	key_identifier_map[GLFW_KEY_F18] = Rocket::Core::Input::KI_F18;
	key_identifier_map[GLFW_KEY_F19] = Rocket::Core::Input::KI_F19;
	key_identifier_map[GLFW_KEY_F20] = Rocket::Core::Input::KI_F20;
	key_identifier_map[GLFW_KEY_F21] = Rocket::Core::Input::KI_F21;
	key_identifier_map[GLFW_KEY_F22] = Rocket::Core::Input::KI_F22;
	key_identifier_map[GLFW_KEY_F23] = Rocket::Core::Input::KI_F23;
	key_identifier_map[GLFW_KEY_F24] = Rocket::Core::Input::KI_F24;

	key_identifier_map[GLFW_KEY_KP_NUM_LOCK] = Rocket::Core::Input::KI_NUMLOCK;
	key_identifier_map[GLFW_KEY_SCROLL_LOCK] = Rocket::Core::Input::KI_SCROLL;

	key_identifier_map[GLFW_KEY_LSHIFT] = Rocket::Core::Input::KI_LSHIFT;
	key_identifier_map[GLFW_KEY_LCTRL] = Rocket::Core::Input::KI_LCONTROL;
	key_identifier_map[GLFW_KEY_MENU] = Rocket::Core::Input::KI_LMENU;
}

int Input::GetKeyModifierState()
{
	int key_modifier_state = 0;

	// Query the state of all modifier keys
	if (GetKeyState(VK_CAPITAL) & 1)
	{
		key_modifier_state |= Rocket::Core::Input::KM_CAPSLOCK;
	}

	if (HIWORD(GetKeyState(VK_SHIFT)) & 1)
	{
		key_modifier_state |= Rocket::Core::Input::KM_SHIFT;
	}

	if (GetKeyState(VK_NUMLOCK) & 1)
	{
		key_modifier_state |= Rocket::Core::Input::KM_NUMLOCK;
	}

	if (HIWORD(GetKeyState(VK_CONTROL)) & 1)
	{
		key_modifier_state |= Rocket::Core::Input::KM_CTRL;
	}

	if (HIWORD(GetKeyState(VK_MENU)) & 1)
	{
		key_modifier_state |= Rocket::Core::Input::KM_ALT;
	}

	return key_modifier_state;
}

// Returns the character code for a key identifer / key modifier combination.
Rocket::Core::word Input::GetCharacterCode(Rocket::Core::Input::KeyIdentifier key_identifier, int key_modifier_state)
{
	// Check if we have a keycode capable of generating characters on the main keyboard (ie, not on the numeric
	// keypad; that is dealt with below).
	if (key_identifier <= Rocket::Core::Input::KI_OEM_102)
	{
		// Get modifier states
		bool shift = (key_modifier_state & Rocket::Core::Input::KM_SHIFT) > 0;
		bool capslock = (key_modifier_state & Rocket::Core::Input::KM_CAPSLOCK) > 0;

		// Return character code based on identifier and modifiers
		if (shift && !capslock)
			return InputAsciiMap::ascii_map[1][key_identifier];

		if (shift && capslock)
			return InputAsciiMap::ascii_map[2][key_identifier];	

		if (!shift && capslock)
			return InputAsciiMap::ascii_map[3][key_identifier];

		return InputAsciiMap::ascii_map[0][key_identifier];
	}

	// Check if we have a keycode from the numeric keypad.
	else if (key_identifier <= Rocket::Core::Input::KI_OEM_NEC_EQUAL)
	{
		if (key_modifier_state & Rocket::Core::Input::KM_NUMLOCK)
			return InputAsciiMap::keypad_map[0][key_identifier - Rocket::Core::Input::KI_NUMPAD0];
		else
			return InputAsciiMap::keypad_map[1][key_identifier - Rocket::Core::Input::KI_NUMPAD0];
	}

	else if (key_identifier == Rocket::Core::Input::KI_RETURN)
		return '\n';

	return 0;
}
