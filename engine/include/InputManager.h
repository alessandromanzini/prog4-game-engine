#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "Singleton.h"

namespace engine
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool process_input( );
	};

}

#endif // INPUTMANAGER_H
