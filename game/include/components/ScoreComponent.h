#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H

// +------------------------------------+
// | PROJECT HEADERS					|
// +------------------------------------+
#include <components/Component.h>
#include <event/Subject.h>


namespace game
{
    class ScoreComponent final : public engine::Component, public engine::Subject
    {
    public:
        explicit ScoreComponent( engine::GameObjectView& owner );

        void add_score( int score );
        [[nodiscard]] int get_score( ) const;

    private:
        static constexpr int WINNING_SCORE_{ 400 };

        int score_{};

    };

}


#endif //!SCORECOMPONENT_H
