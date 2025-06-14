#ifndef GAMEOVERDELEGATECOMPONENT_H
#define GAMEOVERDELEGATECOMPONENT_H

#include <framework/component/Component.h>

#include <framework/event/Subject.h>


namespace game
{
    class GameOverDelegateComponent final : public engine::Component
    {
    public:
        explicit GameOverDelegateComponent( owner_t& owner );
        void add_observer( engine::Observer& observer );
        void remove_observer( const engine::Observer& observer );
        void signal_gameover() const;

    private:
        engine::Subject subject_{};

    };

}


#endif //!GAMEOVERDELEGATECOMPONENT_H
