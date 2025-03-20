set(EMSCRIPTEN_COMPILE_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2 -g")
set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2 -g -sALLOW_MEMORY_GROWTH --preload-file \"${CMAKE_SOURCE_DIR}/game/resources@/\" -sSDL2_IMAGE_FORMATS=['tga','png']")

# work around https://github.com/emscripten-core/emscripten/issues/11513
set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -fno-stack-protector")

# Use the Emscripten toolchain
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/emscripten_toolchain.cmake)

# Additional settings for Emscripten build
set_target_properties(${PROJECT_NAME} PROPERTIES
        COMPILE_FLAGS "${EMSCRIPTEN_COMPILE_FLAGS}"
        LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS}" )

# Have emscripten generate a html page too.
set(CMAKE_EXECUTABLE_SUFFIX ".html")