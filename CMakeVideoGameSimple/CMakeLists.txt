cmake_minimum_required(VERSION 3.10)

project("CMake Video Game")

set(CMAKE_PREFIX_PATH "C:/Users/Owner/vcpkg/installed/x64-windows")
set(VCPKG_TARGET_TRIPLET "x64-windows")

# This tells vcpkg to automatically copy DLLs
set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)

set(SOURCES
	"ColliderComponent.cpp"
	"ECS.cpp"
	"Game.cpp"
	"GameVariables.cpp"
	"main.cpp"
	"Vector2D.cpp"
)

set(HEADERS
	"ColliderComponent.h"
	"Collision.h"
	"ECS.h"
	"Game.h"
	"GameVariables.h"
	"KeyboardController.h"
	"SpriteComponent.h"
	"TextureManager.h"
	"TransformComponent.h"
	"Vector2D.h"
)

add_executable(CMakeVideoGame ${SOURCES} ${HEADERS}   "ButtonComponent.h" "MainMenu.cpp")

target_link_libraries(CMakeVideoGame
	SDL2::SDL2
	SDL2::SDL2main
	SDL2_image::SDL2_image
	SDL2_mixer::SDL2_mixer
)

if(WIN32)
	add_custom_command(TARGET CMakeVideoGame POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_FILE:SDL2::SDL2>
			$<TARGET_FILE_DIR:CMakeVideoGame>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_FILE:SDL2_image::SDL2_image>
			$<TARGET_FILE_DIR:CMakeVideoGame>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_FILE:SDL2_mixer::SDL2_mixer>
			$<TARGET_FILE_DIR:CMakeVideoGame>
	)
	add_custom_command(TARGET CMakeVideoGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            C:/Users/Owner/vcpkg/installed/x64-windows/debug/bin
            $<TARGET_FILE_DIR:CMakeVideoGame>
    )

	add_custom_command(TARGET CMakeVideoGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/assets
)
endif()