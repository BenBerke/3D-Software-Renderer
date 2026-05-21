cmake_minimum_required(VERSION 3.28)
project(SoftwareRenderer C)

set(CMAKE_C_STANDARD 11)

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)

add_executable(SoftwareRenderer
        main.c
        src/InputManager.c
        src/Vector2.c
        src/Matrix3.c
        src/Projection.c
        src/Vector3.c
        Headers/Matrix3.h
        Headers/Projection.h
        Headers/Vector3.h
        Headers/config.h
        src/Texture.c
        Headers/Texture.h
)

target_link_libraries(SoftwareRenderer PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
)