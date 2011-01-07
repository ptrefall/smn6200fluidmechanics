#include "precomp.h"
#include "Input.h"
#include "InputAsciiMap.h"
#include <Core/CoreMgr.h>
#include <Gui/GuiMgr.h>
#include <windows.h>

using namespace Engine;

Rocket::Core::Input::KeyIdentifier Input::key_identifier_map[KEYMAP_SIZE];
int Input::key_glfw[KEYMAP_SIZE];
bool Input::key_pressed[KEYMAP_SIZE];
bool Input::btn_pressed[GLFW_MOUSE_BUTTON_LAST+1];

bool Input::Initialise()
{
	Input::InitialiseKeymap();
	return true;
}

void Input::InitialiseKeymap()
{
	// Initialise the key map with default values.
	memset(key_identifier_map, 0, sizeof(key_identifier_map));
	memset(key_glfw, 0, sizeof(key_glfw));
	memset(key_pressed, 0, sizeof(key_pressed));
	memset(btn_pressed, 0, sizeof(btn_pressed));
	
	// Assign individual values.
	key_glfw[0] = 'A';
	key_glfw[1] = 'B';
	key_glfw[2] = 'C';
	key_glfw[3] = 'D';
	key_glfw[4] = 'E';
	key_glfw[5] = 'F';
	key_glfw[6] = 'G';
	key_glfw[7] = 'H';
	key_glfw[8] = 'I';
	key_glfw[9] = 'J';
	key_glfw[10] = 'K';
	key_glfw[11] = 'L';
	key_glfw[12] = 'M';
	key_glfw[13] = 'N';
	key_glfw[14] = 'O';
	key_glfw[15] = 'P';
	key_glfw[16] = 'Q';
	key_glfw[17] = 'R';
	key_glfw[18] = 'S';
	key_glfw[19] = 'T';
	key_glfw[20] = 'U';
	key_glfw[21] = 'V';
	key_glfw[22] = 'W';
	key_glfw[23] = 'X';
	key_glfw[24] = 'Y';
	key_glfw[25] = 'Z';
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

	key_glfw[26] = '0';
	key_glfw[27] = '1';
	key_glfw[28] = '2';
	key_glfw[29] = '3';
	key_glfw[30] = '4';
	key_glfw[31] = '5';
	key_glfw[32] = '6';
	key_glfw[33] = '7';
	key_glfw[34] = '8';
	key_glfw[35] = '9';
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

	key_glfw[36] = GLFW_KEY_BACKSPACE;
	key_glfw[37] = GLFW_KEY_TAB;
	key_identifier_map[GLFW_KEY_BACKSPACE] = Rocket::Core::Input::KI_BACK;
	key_identifier_map[GLFW_KEY_TAB] = Rocket::Core::Input::KI_TAB;

	key_glfw[38] = GLFW_KEY_ENTER;
	key_identifier_map[GLFW_KEY_ENTER] = Rocket::Core::Input::KI_RETURN;

	key_glfw[39] = GLFW_KEY_PAUSE;
	key_glfw[40] = GLFW_KEY_CAPS_LOCK;
	key_identifier_map[GLFW_KEY_PAUSE] = Rocket::Core::Input::KI_PAUSE;
	key_identifier_map[GLFW_KEY_CAPS_LOCK] = Rocket::Core::Input::KI_CAPITAL;

	key_glfw[41] = GLFW_KEY_ESC;
	key_identifier_map[GLFW_KEY_ESC] = Rocket::Core::Input::KI_ESCAPE;

	key_glfw[42] = GLFW_KEY_SPACE;
	key_glfw[43] = GLFW_KEY_END;
	key_glfw[44] = GLFW_KEY_HOME;
	key_glfw[45] = GLFW_KEY_LEFT;
	key_glfw[46] = GLFW_KEY_UP;
	key_glfw[47] = GLFW_KEY_RIGHT;
	key_glfw[48] = GLFW_KEY_DOWN;
	key_glfw[49] = GLFW_KEY_INSERT;
	key_glfw[50] = GLFW_KEY_DEL;
	key_identifier_map[GLFW_KEY_SPACE] = Rocket::Core::Input::KI_SPACE;
	key_identifier_map[GLFW_KEY_END] = Rocket::Core::Input::KI_END;
	key_identifier_map[GLFW_KEY_HOME] = Rocket::Core::Input::KI_HOME;
	key_identifier_map[GLFW_KEY_LEFT] = Rocket::Core::Input::KI_LEFT;
	key_identifier_map[GLFW_KEY_UP] = Rocket::Core::Input::KI_UP;
	key_identifier_map[GLFW_KEY_RIGHT] = Rocket::Core::Input::KI_RIGHT;
	key_identifier_map[GLFW_KEY_DOWN] = Rocket::Core::Input::KI_DOWN;
	key_identifier_map[GLFW_KEY_INSERT] = Rocket::Core::Input::KI_INSERT;
	key_identifier_map[GLFW_KEY_DEL] = Rocket::Core::Input::KI_DELETE;
	
	key_glfw[51] = GLFW_KEY_KP_0;
	key_glfw[52] = GLFW_KEY_KP_1;
	key_glfw[53] = GLFW_KEY_KP_2;
	key_glfw[54] = GLFW_KEY_KP_3;
	key_glfw[55] = GLFW_KEY_KP_4;
	key_glfw[56] = GLFW_KEY_KP_5;
	key_glfw[57] = GLFW_KEY_KP_6;
	key_glfw[58] = GLFW_KEY_KP_7;
	key_glfw[59] = GLFW_KEY_KP_8;
	key_glfw[60] = GLFW_KEY_KP_9;
	key_glfw[61] = GLFW_KEY_KP_MULTIPLY;
	key_glfw[62] = GLFW_KEY_KP_ADD;
	key_glfw[63] = GLFW_KEY_KP_SUBTRACT;
	key_glfw[64] = GLFW_KEY_KP_DECIMAL;
	key_glfw[65] = GLFW_KEY_KP_DIVIDE;
	key_glfw[66] = GLFW_KEY_F1;
	key_glfw[67] = GLFW_KEY_F2;
	key_glfw[68] = GLFW_KEY_F3;
	key_glfw[69] = GLFW_KEY_F4;
	key_glfw[70] = GLFW_KEY_F5;
	key_glfw[71] = GLFW_KEY_F6;
	key_glfw[72] = GLFW_KEY_F7;
	key_glfw[73] = GLFW_KEY_F8;
	key_glfw[74] = GLFW_KEY_F9;
	key_glfw[75] = GLFW_KEY_F10;
	key_glfw[76] = GLFW_KEY_F11;
	key_glfw[77] = GLFW_KEY_F12;
	key_glfw[78] = GLFW_KEY_F13;
	key_glfw[79] = GLFW_KEY_F14;
	key_glfw[80] = GLFW_KEY_F15;
	key_glfw[81] = GLFW_KEY_F16;
	key_glfw[82] = GLFW_KEY_F17;
	key_glfw[83] = GLFW_KEY_F18;
	key_glfw[84] = GLFW_KEY_F19;
	key_glfw[85] = GLFW_KEY_F20;
	key_glfw[86] = GLFW_KEY_F21;
	key_glfw[87] = GLFW_KEY_F22;
	key_glfw[88] = GLFW_KEY_F23;
	key_glfw[89] = GLFW_KEY_F24;
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

	key_glfw[90] = GLFW_KEY_KP_NUM_LOCK;
	key_glfw[91] = GLFW_KEY_SCROLL_LOCK;
	key_identifier_map[GLFW_KEY_KP_NUM_LOCK] = Rocket::Core::Input::KI_NUMLOCK;
	key_identifier_map[GLFW_KEY_SCROLL_LOCK] = Rocket::Core::Input::KI_SCROLL;

	key_glfw[92] = GLFW_KEY_LSHIFT;
	key_glfw[93] = GLFW_KEY_LCTRL;
	key_glfw[94] = GLFW_KEY_MENU;
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
