#ifndef RENDERER_H
#define RENDERER_H

// +--------------------------------+
// | SDL HEADERS					|
// +--------------------------------+
#include <SDL.h>

// +--------------------------------+
// | PROJECT HEADERS				|
// +--------------------------------+
#include <singletons/Singleton.h>
#include <singletons/UIController.h>

#include <glm.hpp>


namespace engine
{
    class Texture2D;
    /**
     * Simple RAII wrapper for the SDL renderer
     */
    class Renderer final : public Singleton<Renderer>
    {
        friend class Singleton;

    public:
        void init( SDL_Window* window );
        void render( ) const;
        void destroy( );

        void render_texture( const Texture2D& texture, glm::vec2 position ) const;
        void render_texture( const Texture2D& texture, const glm::vec4& dstRect ) const;

        void render_partial_texture( const Texture2D& texture, glm::vec2 position, const glm::vec4& srcRect ) const;
        void render_partial_texture( const Texture2D& texture, const glm::vec4& dstRect, const glm::vec4& srcRect ) const;

        [[nodiscard]] SDL_Renderer* get_SDL_renderer( ) const;

        [[nodiscard]] const SDL_Color& get_background_color( ) const;

        void set_background_color( const SDL_Color& color );

        friend void UIController::init( );

    private:
        SDL_Renderer* renderer_ptr_{};
        SDL_Window* window_ptr_{};
        SDL_Color clear_color_{};

        Renderer( ) = default;

        void render_texture_ex_impl( SDL_Texture* texture, const SDL_Rect* dst, SDL_Rect* src ) const;
        void render_texture_impl( SDL_Texture* texture, const SDL_Rect* dst, const SDL_Rect* src ) const;

    };

    extern Renderer& RENDERER;

}

#endif //!RENDERER_H
