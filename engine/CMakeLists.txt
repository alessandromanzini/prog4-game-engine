# Engine CMakeLists.txt, Author: Alessandro Manzini
# Engine library configuration specifics.
#
project(engine)

# create library target
add_library(${PROJECT_NAME}
	    "src/Font.cpp"
        "src/FPS.cpp"
        "src/GameObject.cpp"
        "src/GameObjectView.cpp"
        "src/GameTime.cpp"
        "src/InputManager.cpp"
        "src/Minigin.cpp"
        "src/Renderer.cpp"
        "src/ResourceManager.cpp"
        "src/Scene.cpp"
        "src/SceneManager.cpp"
        "src/TextComponent.cpp"
        "src/TextureComponent.cpp"
        "src/Texture2D.cpp"
        "src/Transform.cpp" )

# add glm
include(glm_fetchcontent)

# add SDL2
if (WIN32)
    include(sdl_win_fetchcontent)

    find_package(VLD CONFIG)

    # set all include directories
    target_include_directories(${PROJECT_NAME} PUBLIC
	    ${VLD_INCLUDE_DIR}
	    ${SDL2_INCLUDE_DIR} 
	    ${SDL2_IMAGE_INCLUDE_DIR} 
	    ${SDL2_TTF_INCLUDE_DIR} )
  
    # set libraries to link with
    target_link_libraries(${PROJECT_NAME} PUBLIC
	    ${SDL2_LIBRARIES} 
	    ${SDL2_IMAGE_LIBRARIES} 
	    ${SDL2_TTF_LIBRARIES} 
	    ${VLD_LIBRARY} )

elseif(NOT EMSCRIPTEN)
    include(sdl_dwn_fetchcontent)

    # set all include directories
    target_include_directories(${PROJECT_NAME} PUBLIC 
        ${sdl2_SOURCE_DIR}/include
        ${sdl2_image_SOURCE_DIR}/include
        ${sdl2_ttf_SOURCE_DIR} )

    # set libraries to link with
    target_link_libraries(${PROJECT_NAME} PUBLIC 
        SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf )
endif()

# the include folder and resources has headers that will be shared to external project.
# the src folder also need to be included in case we have 
# headers for internal use.
target_include_directories(${PROJECT_NAME}
		PUBLIC "./include"
		PRIVATE "./src"
        PUBLIC ${glm_SOURCE_DIR} )

# setup the target properties
if(EMSCRIPTEN)
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
  
elseif(WIN32)
  set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${GAME_BINARY_PATH})

  # add post build commands
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_LIBRARY_DLL} ${GAME_BINARY_PATH} )
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_IMAGE_LIBRARY_DLL} ${GAME_BINARY_PATH} )
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_TTF_LIBRARY_DLL} ${GAME_BINARY_PATH} )
endif()
