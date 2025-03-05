#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "Singleton.h"

namespace engine
{
	class InputSystem final : public Singleton<InputSystem>
	{
	public:
		bool process_input( );

	private:
		class InputSystemImpl;
		InputSystemImpl* impl_ptr_{ nullptr };

	};

	extern InputSystem& INPUT_SYSTEM;

}

#endif // INPUTSYSTEM_H
