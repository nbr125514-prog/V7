cmake_minimum_required(VERSION 3.20)

project(AimTrainer LANGUAGES CXX)

add_executable(AimTrainer WIN32 main.cpp)

if(MSVC)
    set_property(TARGET AimTrainer PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreadedDLL"
    )

    target_compile_options(AimTrainer PRIVATE /W4 /permissive-)
    target_link_options(AimTrainer PRIVATE /SUBSYSTEM:WINDOWS)
endif()

target_compile_features(AimTrainer PRIVATE cxx_std_17)

target_compile_definitions(AimTrainer PRIVATE
    UNICODE
    _UNICODE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
)
