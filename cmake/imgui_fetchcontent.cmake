include(FetchContent)

# Fetch Dear ImGui
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG master
)
FetchContent_MakeAvailable(imgui)

target_include_directories(${PROJECT_NAME}
        PUBLIC ${imgui_SOURCE_DIR}
        PUBLIC ${imgui_SOURCE_DIR}/backends)

# add required source files for imgui
target_sources(${PROJECT_NAME} PRIVATE
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_SDL2.cpp)

if (APPLE)
    # Would be better with Metal, but sdl2 renderer will do for now
    target_sources(${PROJECT_NAME} PRIVATE ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp)

else ()
    target_sources(${PROJECT_NAME} PRIVATE ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)

endif ()

# tell the program that we are using imgui
# target_compile_definitions(${PROJECT_NAME} PUBLIC _USE_IMGUI)
# message( STATUS "Debug mode enabled, _USE_IMGUI defined" )
# endif()
