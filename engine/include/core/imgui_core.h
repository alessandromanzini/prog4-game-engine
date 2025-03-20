#ifndef IMGUI_CORE_H
#define IMGUI_CORE_H

// +--------------------------------+
// | SDL HEADERS					|
// +--------------------------------+
#include <SDL_events.h>

// +--------------------------------+
// | FORWARD DECLARATIONS			|
// +--------------------------------+
struct SDL_Renderer;
struct ImDrawData;


namespace imgui_core
{
    void init( SDL_Window* window, SDL_Renderer* renderer );
    void destroy( );

    void process_input( const SDL_Event& event );

    void new_frame( );
    void render( ImDrawData* data, SDL_Renderer* renderer );
}


#endif //!IMGUI_CORE_H
