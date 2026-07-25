cmake_minimum_required(VERSION 3.20)

project(MyApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY
        "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Prefer the Config package (vcpkg, Homebrew), otherwise fall back
# to the Find module (Ubuntu, Debian, etc.)
find_package(wxWidgets CONFIG QUIET)

if(wxWidgets_FOUND)
    add_executable(DMirror WIN32
        src/main.cpp
        src/platform/windows/app.rc
        src/resources/icon.h
    )

    target_link_libraries(DMirror PRIVATE
        wx::core
        wx::base
    )
else()
    find_package(wxWidgets REQUIRED COMPONENTS core base)
    include(${wxWidgets_USE_FILE})

    add_executable(DMirror
        src/main.cpp
        src/resources/icon.h
    )

    target_link_libraries(DMirror PRIVATE ${wxWidgets_LIBRARIES})
    target_compile_definitions(DMirror PRIVATE ${wxWidgets_DEFINITIONS})
endif()
