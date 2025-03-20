# set maximum warning level and treat warnings as errors
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

message(STATUS "${PROJECT_NAME}: W4 warning level and treat warnings as errors.")
