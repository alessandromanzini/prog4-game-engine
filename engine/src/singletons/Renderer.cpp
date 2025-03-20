#include "singletons/Renderer.h"

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "Texture2D.h"
#include "singletons/ScenePool.h"
#include "singletons/UIController.h"

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <cstring>
#include <stdexcept>


namespace engine
{
    Renderer& RENDERER = Renderer::get_instance( );


    int GetOpenGLDriverIndex( )
    {
        auto openglIndex       = -1;
        const auto driverCount = SDL_GetNumRenderDrivers( );
        for ( auto i = 0; i < driverCount; i++ )
        {
            SDL_RendererInfo info;
            if ( !SDL_GetRenderDriverInfo( i, &info ) )
            {
                if ( !strcmp( info.name, "opengl" ) )
                {
                    openglIndex = i;
                }
            }
        }
        return openglIndex;
    }


    void Renderer::init( SDL_Window* window )
    {
        window_ptr_   = window;
        renderer_ptr_ = SDL_CreateRenderer( window, GetOpenGLDriverIndex( ), SDL_RENDERER_ACCELERATED );
        if ( renderer_ptr_ == nullptr )
        {
            throw std::runtime_error( std::string( "SDL_CreateRenderer Error: " ) + SDL_GetError( ) );
        }
    }


    void Renderer::render( ) const
    {
        const auto& [r, g, b, a] = get_background_color( );

        SDL_SetRenderDrawColor( renderer_ptr_, r, g, b, a );
        SDL_RenderClear( renderer_ptr_ );

        SCENE_POOL.render( );
        UI_CONTROLLER.render( );

        SDL_RenderPresent( renderer_ptr_ );
    }


    void Renderer::destroy( )
    {
        if ( renderer_ptr_ != nullptr )
        {
            SDL_DestroyRenderer( renderer_ptr_ );
            renderer_ptr_ = nullptr;
        }
    }


    void Renderer::render_texture( const Texture2D& texture, const float x, const float y ) const
    {
        SDL_Rect dst{};
        dst.x = static_cast<int>( x );
        dst.y = static_cast<int>( y );
        SDL_QueryTexture( texture.get_SDL_texture( ), nullptr, nullptr, &dst.w, &dst.h );
        SDL_RenderCopy( get_SDL_renderer( ), texture.get_SDL_texture( ), nullptr, &dst );
    }


    void Renderer::render_texture( const Texture2D& texture, const float x, const float y, const float width,
                                   const float height ) const
    {
        SDL_Rect dst{};
        dst.x = static_cast<int>( x );
        dst.y = static_cast<int>( y );
        dst.w = static_cast<int>( width );
        dst.h = static_cast<int>( height );
        SDL_RenderCopy( get_SDL_renderer( ), texture.get_SDL_texture( ), nullptr, &dst );
    }


    SDL_Renderer* Renderer::get_SDL_renderer( ) const
    {
        return renderer_ptr_;
    }


    const SDL_Color& Renderer::get_background_color( ) const
    {
        return clear_color_;
    }


    void Renderer::set_background_color( const SDL_Color& color )
    {
        clear_color_ = color;
    }

}
