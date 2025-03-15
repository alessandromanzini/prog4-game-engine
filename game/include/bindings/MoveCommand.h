#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include "Command.h"

namespace game
{
	class MoveCommand final : public engine::ComponentCommand
	{
	public:
		MoveCommand( engine::Component& component )
			: engine::ComponentCommand( component )
		{
		}

		void execute( ) override
		{
			int a = 10;
			a += 10;
		}

	};

}

#endif // !MOVECOMMAND_H