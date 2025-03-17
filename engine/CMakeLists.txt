# Engine CMakeLists.txt, Author: Alessandro Manzini
# Engine library configuration specifics.
#
project(engine)

# create library target
add_library(${PROJECT_NAME}
        "src/bindings/InputMappingContext.cpp"

        "src/event/Subject.cpp"

        "include/scene_graph/Deleter.h"
        "src/scene_graph/GameObject.cpp"
        "src/scene_graph/GameObjectView.cpp"
        "src/scene_graph/Scene.cpp"
        "src/scene_graph/Transform.cpp"

        "src/scene_graph/components/FPS.cpp"
        "src/scene_graph/components/InputDisplayUIComponent.cpp"
        "src/scene_graph/components/TextComponent.cpp"
        "src/scene_graph/components/TextureComponent.cpp"

	    "src/framework/Font.cpp"
        "src/framework/Minigin.cpp"
        "src/framework/Texture2D.cpp"

        "src/singletons/GameTime.cpp"
        "src/singletons/InputSystem.cpp"
        "src/singletons/Renderer.cpp"
        "src/singletons/ResourceManager.cpp"
        "src/singletons/ScenePool.cpp"
        "src/singletons/UIController.cpp" )

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
		PUBLIC "./include/bindings"
		PUBLIC "./include/event"
		PUBLIC "./include/framework"
		PUBLIC "./include/scene_graph"
		PUBLIC "./include/scene_graph/components"
		PUBLIC "./include/singletons"
		PRIVATE "./src"
		PRIVATE "./src/bindings"
		PRIVATE "./src/event"
		PRIVATE "./src/framework"
		PRIVATE "./src/scene_graph"
		PRIVATE "./src/scene_graph/components"
		PRIVATE "./src/singletons"
        PUBLIC ${glm_SOURCE_DIR} )

# add debug-only dependencies
# if (CMAKE_BUILD_TYPE STREQUAL "Debug")
# add imgui
include(imgui_fetchcontent)

target_include_directories( ${PROJECT_NAME} 
        PUBLIC ${imgui_SOURCE_DIR}
        PUBLIC ${imgui_SOURCE_DIR}/backends )

# add required source files for imgui
target_sources(${PROJECT_NAME} PRIVATE 
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_SDL2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp )

# tell the program that we are using imgui
# target_compile_definitions(${PROJECT_NAME} PUBLIC _USE_IMGUI)
# message( STATUS "Debug mode enabled, _USE_IMGUI defined" )
# endif()

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
