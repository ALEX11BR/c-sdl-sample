cmake_minimum_required(VERSION 3.12)

project(c-sdl-sample)
add_executable(${PROJECT_NAME} main.c)

option(DISABLE_PKGCONFIG "Disable using pkg-config for finding SDL2 flags, using instead CMake for this" OFF)

if(DEFINED EMSCRIPTEN)
    target_compile_options(${PROJECT_NAME} PRIVATE
        "--use-port=sdl2" "--use-port=sdl2_image:formats=png"
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        "--use-port=sdl2" "--use-port=sdl2_image:formats=png"
        "-sWASM=1" "--preload-file=assets"
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
else()
    if(WIN32)
        set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON)
    endif()

    find_package(PkgConfig)
    if(PkgConfig_FOUND AND NOT DISABLE_PKGCONFIG)
        pkg_check_modules(SDL2 REQUIRED sdl2 SDL2_image)

        target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
        target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
        target_compile_options(${PROJECT_NAME} PRIVATE ${SDL2_CFLAGS_OTHER})
    else()
        find_package(SDL2 CONFIG REQUIRED)
        find_package(SDL2_image CONFIG REQUIRED)

        target_link_libraries(${PROJECT_NAME} PRIVATE
            $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
            SDL2::SDL2 SDL2_image::SDL2_image
        )
    endif()
endif()

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(${PROJECT_NAME} copy_assets)
