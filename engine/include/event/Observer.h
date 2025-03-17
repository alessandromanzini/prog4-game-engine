#ifndef OBSERVER_H
#define OBSERVER_H

#include "Subject.h"
#include "framework/UID.h"

namespace engine
{
	class Observer
	{
		friend class Subject;

	public:
		virtual void notify( UID event, Subject* subject, event::broadcast_value_variant_t value ) = 0;

	private:
		Observer* next_ptr_{ nullptr };

	};

}

#endif // !OBSERVER_H
