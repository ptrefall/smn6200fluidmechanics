#pragma once

namespace Engine
{
	class CoreMgr;

	class Input
	{
	public:
		static bool Initialise(CoreMgr *coreManager);

	private:
		static void InitialiseKeymap();

		static void processKeyboardEvent(int key, int action);
		static void processCharEvent(int key, int action);
		static void processMouseButtonEvent(int button, int action);
		static void processMouseMoveEvent(int x, int y);

		static int GetKeyModifierState();

		/// Returns the character code for a key identifer / key modifier combination.
		/// @param[in] key_identifier The key to generate a character code for.
		/// @param[in] key_modifier_state The configuration of the key modifiers.
		/// @return The character code.
		static Rocket::Core::word GetCharacterCode(Rocket::Core::Input::KeyIdentifier key_identifier, int key_modifier_state);
	};
}
