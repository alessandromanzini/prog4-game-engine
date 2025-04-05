#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H

// +------------------------------------+
// | PROJECT HEADERS					|
// +------------------------------------+
#include <components/Component.h>
#include <event/Subject.h>


namespace game
{
    class ScoreComponent final : public engine::Component
    {
    public:
        explicit ScoreComponent( engine::GameObjectView& owner );

        void add_score( int score );
        [[nodiscard]] int get_score( ) const;

    private:
        static constexpr int WINNING_SCORE_{ 400 };

        int score_{};
        engine::Subject score_subject_{};

    };

}


#endif //!SCORECOMPONENT_H
