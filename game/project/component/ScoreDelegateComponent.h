#ifndef SCOREDELEGATECOMPONENT_H
#define SCOREDELEGATECOMPONENT_H

#include <framework/component/Component.h>


namespace game
{
    class ScoreComponent;
    class ScoreDelegateComponent final : public engine::Component
    {
    public:
        explicit ScoreDelegateComponent( owner_t& owner, ScoreComponent* score );
        void increase_score( int value ) const;
        void signal_player_death( ) const;

    private:
        ScoreComponent* score_component_ptr_{ nullptr };

    };

}


#endif //!SCOREDELEGATECOMPONENT_H
