#include <stdexcept>
#include <cstring>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"

using namespace engine;

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
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

void Renderer::init(SDL_Window* window)
{
	window_ptr_ = window;
	renderer_ptr_ = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if ( renderer_ptr_ == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void Renderer::render() const
{
	const auto& color = get_background_color();

	SDL_SetRenderDrawColor(renderer_ptr_, color.r, color.g, color.b, color.a);
	SDL_RenderClear(renderer_ptr_);

	SceneManager::get_instance().render();
	
	SDL_RenderPresent( renderer_ptr_ );
}

void Renderer::destroy()
{
	if (renderer_ptr_ != nullptr)
	{
		SDL_DestroyRenderer( renderer_ptr_ );
		renderer_ptr_ = nullptr;
	}
}

void Renderer::render_texture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.get_SDL_texture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(get_SDL_renderer(), texture.get_SDL_texture(), nullptr, &dst);
}

void Renderer::render_texture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(get_SDL_renderer(), texture.get_SDL_texture(), nullptr, &dst);
}

SDL_Renderer* Renderer::get_SDL_renderer() const { return renderer_ptr_; }
