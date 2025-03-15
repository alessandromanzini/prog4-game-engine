#ifndef COMMAND_H
#define COMMAND_H

#include "components/Component.h"

namespace engine
{
	// +--------------------------------+
	// | BASE COMMAND					|
	// +--------------------------------+
	class Command
	{
	public:
		virtual ~Command( ) noexcept = default;
		virtual void execute( ) = 0;

	};

	// +--------------------------------+
	// | BASE COMPONENT COMMAND			|
	// +--------------------------------+
	class ComponentCommand : public Command
	{
	public:
		ComponentCommand( Component& target )
			: target_{ target }
		{
		}
		virtual ~ComponentCommand( ) noexcept = default;

	protected:
		[[nodiscard]] Component& get_target( ) const
		{
			return target_;
		}

	private:
		Component& target_;

	};

}

#endif // !COMMAND_H