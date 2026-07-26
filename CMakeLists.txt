cmake_minimum_required(VERSION 3.20)

project(MyApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY
        "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

list(APPEND SOURCES)

# Prefer the Config package (vcpkg, Homebrew), otherwise fall back
# to the Find module (Ubuntu, Debian, etc.)
find_package(wxWidgets CONFIG QUIET)

if(wxWidgets_FOUND)
    add_executable(DMirror WIN32
        ${SOURCES}
        src/platform/windows/app.rc
    )

    target_link_libraries(DMirror PRIVATE
        wx::core
        wx::base
    )
else()
    find_package(wxWidgets REQUIRED COMPONENTS core base)
    include(${wxWidgets_USE_FILE})

    add_executable(DMirror ${SOURCES})

    target_link_libraries(DMirror PRIVATE ${wxWidgets_LIBRARIES})
    target_compile_definitions(DMirror PRIVATE ${wxWidgets_DEFINITIONS})
endif()

target_include_directories(DMirror PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/resources
)