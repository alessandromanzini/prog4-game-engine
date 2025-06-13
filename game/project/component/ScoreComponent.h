#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H

#include <framework/component/Component.h>
#include <framework/event/Subject.h>


namespace game
{
    enum class ScoreEvents
    {
        SCORE_INCREASED,
        PLAYER_DEATH
    };

    class ScoreComponent final : public engine::Component
    {
    public:
        explicit ScoreComponent( owner_t& owner );

        [[nodiscard]] bool is_high_score( ) const;
        [[nodiscard]] int get_score( ) const;

        void increase_score( int value );
        void signal_player_death( );

        void add_observer( engine::Observer& observer );
        void remove_observer( const engine::Observer& observer );

    private:
        engine::Subject subject_;
        int score_{ 0 };
        int player_lives_{ 3 };

    };

}


#endif //!SCORECOMPONENT_H
