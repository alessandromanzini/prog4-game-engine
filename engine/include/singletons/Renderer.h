#ifndef RENDERER_H
#define RENDERER_H

// +--------------------------------+
// | SDL HEADERS					|
// +--------------------------------+
#include <SDL.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include "Singleton.h"
#include "UIController.h"


namespace engine
{
    class Texture2D;
    /**
     * Simple RAII wrapper for the SDL renderer
     */
    class Renderer final : public Singleton<Renderer>
    {
    public:
        void init( SDL_Window* window );
        void render( ) const;
        void destroy( );

        void render_texture( const Texture2D& texture, float x, float y ) const;
        void render_texture( const Texture2D& texture, float x, float y, float width, float height ) const;

        [[nodiscard]] SDL_Renderer* get_SDL_renderer( ) const;

        [[nodiscard]] const SDL_Color& get_background_color( ) const;

        void set_background_color( const SDL_Color& color );

        friend void UIController::init( );

    private:
        SDL_Renderer* renderer_ptr_{};
        SDL_Window* window_ptr_{};
        SDL_Color clear_color_{};

    };

    extern Renderer& RENDERER;

}

#endif // !RENDERER_H
