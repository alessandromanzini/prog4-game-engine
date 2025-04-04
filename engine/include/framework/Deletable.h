#ifndef DELETABLE_H
#define DELETABLE_H


namespace engine
{
    class Deletable
    {
    public:
        virtual ~Deletable( ) = default;


        virtual void mark_for_deletion( )
        {
            marked_for_deletion_ = true;
        }


        [[nodiscard]] bool is_marked_for_deletion( ) const noexcept
        {
            return marked_for_deletion_;
        }

    protected:
        Deletable( ) = default;

    private:
        bool marked_for_deletion_{ false };

    };
}

#endif //!DELETABLE_H
