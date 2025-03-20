#ifndef COMMAND_H
#define COMMAND_H

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
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
        explicit ComponentCommand( Component& target )
            : target_{ target } { }


        ~ComponentCommand( ) noexcept override = default;

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
