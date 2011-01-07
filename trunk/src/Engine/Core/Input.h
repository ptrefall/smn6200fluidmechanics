#pragma once

namespace Engine
{
	class CoreMgr;

	class Input
	{
	public:
		static bool Initialise();
		static int GetKeyModifierState();

		static const int KEYMAP_SIZE = 256;
		
		static Rocket::Core::Input::KeyIdentifier getKeyId(int i) { return key_identifier_map[i]; }
		static int getKeyGLFW(int i) { return key_glfw[i]; }

		static bool isKeyPressed(int i) { return key_pressed[i]; }
		static void keyPress(int i) { key_pressed[i] = true; }
		static void keyRelease(int i) { key_pressed[i] = false; }

		static bool isButtonPressed(int i) { return btn_pressed[i]; }
		static void buttonPress(int i) { btn_pressed[i] = true; }
		static void buttonRelease(int i) { btn_pressed[i] = false; }
	private:
		static void InitialiseKeymap();

		/// Returns the character code for a key identifer / key modifier combination.
		/// @param[in] key_identifier The key to generate a character code for.
		/// @param[in] key_modifier_state The configuration of the key modifiers.
		/// @return The character code.
		static Rocket::Core::word GetCharacterCode(Rocket::Core::Input::KeyIdentifier key_identifier, int key_modifier_state);

		static Rocket::Core::Input::KeyIdentifier key_identifier_map[KEYMAP_SIZE];
		static int key_glfw[KEYMAP_SIZE];
		static bool key_pressed[KEYMAP_SIZE];
		static bool btn_pressed[GLFW_MOUSE_BUTTON_LAST+1];
	};
}
