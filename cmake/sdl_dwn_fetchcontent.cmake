include(FetchContent)

# SDL2
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.10
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE )
FetchContent_MakeAvailable(SDL2)

# SDL2 Image
FetchContent_Declare(
    SDL2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-2.8.3
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE )

set(SDL2IMAGE_INSTALL OFF)
FetchContent_MakeAvailable(SDL2_image)

# SDL2 ttf
FetchContent_Declare(
    SDL2_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-2.22.0
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE )

set(SDL2TTF_INSTALL OFF)
FetchContent_MakeAvailable(SDL2_ttf)

# SDL2 Mixer
FetchContent_Declare(
    SDL2_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG release-2.8.0
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE )

set(SDL2MIXER_INSTALL OFF)
set(SDL2MIXER_VENDORED ON)           # use bundled libraries when possible
set(SDL2MIXER_OPUS OFF)              # disable Opus support
FetchContent_MakeAvailable(SDL2_mixer)

# set all include directories
target_include_directories(${PROJECT_NAME}
        PUBLIC ${sdl2_SOURCE_DIR}/include
        PUBLIC ${sdl2_image_SOURCE_DIR}/include
        PUBLIC ${sdl2_ttf_SOURCE_DIR}
        PUBLIC ${sdl2_mixer_SOURCE_DIR}/include )

# set libraries to link with
target_link_libraries(${PROJECT_NAME}
        PUBLIC SDL2::SDL2main
        PUBLIC SDL2::SDL2
        PUBLIC SDL2_image::SDL2_image
        PUBLIC SDL2_ttf::SDL2_ttf
        PUBLIC SDL2_mixer::SDL2_mixer
        PUBLIC $ENV{FREETYPE} )
