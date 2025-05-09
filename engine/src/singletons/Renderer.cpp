#include <singletons/Renderer.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <framework/resources/Texture2D.h>
#include <singletons/ScenePool.h>
#include <singletons/UIController.h>

// +--------------------------------+
// | STANDARD HEADERS				|
// +--------------------------------+
#include <cstring>
#include <stdexcept>


namespace engine
{
    Renderer& RENDERER = Renderer::get_instance( );

    // +---------------------------+
    // | HELPERS                   |
    // +---------------------------+
    int get_open_gl_driver_index( )
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


    SDL_Rect create_rect( const glm::vec4& rect )
    {
        return SDL_Rect{
            .x = static_cast<int>( rect.x ),
            .y = static_cast<int>( rect.y ),
            .w = static_cast<int>( rect.z ),
            .h = static_cast<int>( rect.w )
        };
    }


    SDL_Rect create_rect( const glm::vec2 pos )
    {
        return SDL_Rect{
            .x = static_cast<int>( pos.x ),
            .y = static_cast<int>( pos.y )
        };
    }


    // +---------------------------+
    // | RENDERER                  |
    // +---------------------------+
    void Renderer::init( SDL_Window* window )
    {
        window_ptr_   = window;
        renderer_ptr_ = SDL_CreateRenderer( window, get_open_gl_driver_index( ), SDL_RENDERER_ACCELERATED );
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


    void Renderer::render_texture( const Texture2D& texture, const glm::vec2 position ) const
    {
        SDL_Rect dst = create_rect( position );
        SDL_QueryTexture( texture.get_SDL_texture( ), nullptr, nullptr, &dst.w, &dst.h );
        SDL_RenderCopy( get_SDL_renderer( ), texture.get_SDL_texture( ), nullptr, &dst );
    }


    void Renderer::render_texture( const Texture2D& texture, const glm::vec4& dstRect ) const
    {
        const SDL_Rect dst = create_rect( dstRect );
        SDL_RenderCopy( get_SDL_renderer( ), texture.get_SDL_texture( ), nullptr, &dst );
    }


    void Renderer::render_partial_texture( const Texture2D& texture, const glm::vec2 position, const glm::vec4& srcRect ) const
    {
        const SDL_Rect src = create_rect( srcRect );
        SDL_Rect dst       = create_rect( position );
        dst.w = src.w;
        dst.h = src.h;
        SDL_RenderCopy( get_SDL_renderer( ), texture.get_SDL_texture( ), &src, &dst );
    }


    void Renderer::render_partial_texture( const Texture2D& texture,
                                           const glm::vec4& dstRect,
                                           const glm::vec4& srcRect ) const
    {
        const SDL_Rect src = create_rect( srcRect );
        const SDL_Rect dst = create_rect( dstRect );
        SDL_RenderCopy( get_SDL_renderer( ), texture.get_SDL_texture( ), &src, &dst );
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
