#ifndef SINGLETON_H
#define SINGLETON_H

namespace engine
{
    template<typename T>
    class Singleton
    {
    public:
        virtual ~Singleton( ) = default;

        Singleton( const Singleton& )                = delete;
        Singleton( Singleton&& ) noexcept            = delete;
        Singleton& operator=( const Singleton& )     = delete;
        Singleton& operator=( Singleton&& ) noexcept = delete;


        static T& get_instance( )
        {
            static T instance{};
            return instance;
        }

    protected:
        Singleton( ) = default;

    };
}

#endif //!SINGLETON_H
