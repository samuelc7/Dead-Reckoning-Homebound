cmake_minimum_required(VERSION 3.20)
project(DeadReckoningHomebound VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL3 configuration
set(SDL_SHARED ON CACHE BOOL "" FORCE)
set(SDL_STATIC OFF CACHE BOOL "" FORCE)
set(SDL_TESTS OFF CACHE BOOL "" FORCE)

# Build SDL3 from the local source directory
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)

# GLAD (compiled directly into the project)
add_library(glad STATIC
    external/glad/src/glad.c
)
target_include_directories(glad PUBLIC
    external/glad/include
)

add_executable(DeadReckoningHomebound 
    src/camera.cpp
    src/main.cpp 
    src/shader.cpp
    src/stb_image.cpp
)

target_include_directories(DeadReckoningHomebound PRIVATE 
    include
    external/stb_image
    external/glm
)

file(GLOB shaders "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*")
message(status Copy shaders)
foreach(shader ${shaders})
    message(status "From ${shader}")
    get_filename_component(outputFileName ${shader} NAME)
    message(status "To ${CMAKE_CURRENT_BINARY_DIR}/shaders/${outputFileName}")
    configure_file(${shader} ${CMAKE_CURRENT_BINARY_DIR}/shaders/${outputFileName} COPYONLY)
endforeach()


# Link SDL3, GLAD, and OpenGL
target_link_libraries(DeadReckoningHomebound PRIVATE
    SDL3::SDL3
    glad
    opengl32
)

if(MSVC)
	target_compile_options(DeadReckoningHomebound PRIVATE /W4)
endif()

# Copy SDL3.dll beside the executable
if(WIN32 AND SDL_SHARED)
    add_custom_command(
        TARGET DeadReckoningHomebound POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:SDL3::SDL3-shared>"
            "$<TARGET_FILE_DIR:DeadReckoningHomebound>"
        VERBATIM
    )
endif()

# assets
add_custom_command(TARGET DeadReckoningHomebound POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:DeadReckoningHomebound>/assets"
)