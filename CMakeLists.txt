cmake_minimum_required(VERSION 3.20)

set(DMIRROR_VERSION "0.1.0")
if(DEFINED ENV{DMIRROR_VERSION})
    string(REGEX REPLACE "^v" "" DMIRROR_VERSION "$ENV{DMIRROR_VERSION}")
endif()

project(DMirror VERSION ${DMIRROR_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
file(WRITE "${CMAKE_BINARY_DIR}/package-version.txt" "${PROJECT_VERSION}")

if(MSVC AND NOT DEFINED CMAKE_MSVC_RUNTIME_LIBRARY)
    set(CMAKE_MSVC_RUNTIME_LIBRARY
        "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

if(WIN32)
    list(APPEND SOURCES src/platform/windows/app.rc)
endif()

# Prefer the Config package (vcpkg, Homebrew), otherwise fall back
# to the Find module (Ubuntu, Debian, etc.)
find_package(wxWidgets CONFIG QUIET)

if(WIN32)
    add_executable(DMirror WIN32
        ${SOURCES}
    )
elseif(APPLE)
    add_executable(DMirror MACOSX_BUNDLE ${SOURCES})
else()
    add_executable(DMirror ${SOURCES})
endif()

if(wxWidgets_FOUND)
    target_link_libraries(DMirror PRIVATE
        wx::core
        wx::base
    )
else()
    find_package(wxWidgets REQUIRED COMPONENTS core base)
    include(${wxWidgets_USE_FILE})

    target_link_libraries(DMirror PRIVATE ${wxWidgets_LIBRARIES})
    target_compile_definitions(DMirror PRIVATE ${wxWidgets_DEFINITIONS})
endif()

target_include_directories(DMirror PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/resources
)

if(WIN32)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_PACKAGE_EXECUTABLES "DMirror" "DMirror")
    set(CPACK_CREATE_DESKTOP_LINKS "DMirror")
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_SYSTEM_NAME "macOS-${CMAKE_SYSTEM_PROCESSOR}")
elseif(UNIX)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_NAME "dmirror")
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64)$")
        set(DMIRROR_LINUX_ARCH x64)
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64)$")
        set(DMIRROR_LINUX_ARCH arm64)
    else()
        set(DMIRROR_LINUX_ARCH "${CMAKE_SYSTEM_PROCESSOR}")
    endif()
    set(CPACK_DEBIAN_FILE_NAME
        "dmirror_${PROJECT_VERSION}_linux_${DMIRROR_LINUX_ARCH}.deb")
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
endif()

set(CPACK_PACKAGE_NAME "DMirror")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "Tamas Henninger-Varadi")
set(CPACK_PACKAGE_CONTACT "1997tamas.varadi@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Cross-platform backup application")
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/packages")

if(WIN32)
    install(TARGETS DMirror RUNTIME DESTINATION .)
    install(FILES README.md DESTINATION .)

    if(WX_RUNTIME_DIR)
        file(GLOB WX_RUNTIME_DLLS "${WX_RUNTIME_DIR}/*.dll")
        if(NOT WX_RUNTIME_DLLS)
            message(FATAL_ERROR "No wxWidgets DLLs found in ${WX_RUNTIME_DIR}")
        endif()
        install(FILES ${WX_RUNTIME_DLLS} DESTINATION .)
    endif()

    if(WX_LICENSE_FILE)
        if(NOT EXISTS "${WX_LICENSE_FILE}")
            message(FATAL_ERROR "wxWidgets license not found: ${WX_LICENSE_FILE}")
        endif()
        install(FILES "${WX_LICENSE_FILE}"
            DESTINATION .
            RENAME wxWidgets-LICENSE.txt
        )
    endif()

    if(MSVC AND CMAKE_MSVC_RUNTIME_LIBRARY MATCHES "DLL")
        set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION .)
        include(InstallRequiredSystemLibraries)
    endif()
elseif(APPLE)
    install(TARGETS DMirror BUNDLE DESTINATION .)
    install(FILES README.md DESTINATION .)

    install(CODE [[
        include(BundleUtilities)
        set(bundle "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/DMirror.app")
        get_filename_component(bundle "${bundle}" ABSOLUTE)
        file(GLOB bundle_search_dirs LIST_DIRECTORIES true
            "$ENV{HOMEBREW_PREFIX}/opt/*/lib"
        )
        list(PREPEND bundle_search_dirs "$ENV{HOMEBREW_PREFIX}/lib")
        fixup_bundle("${bundle}" "" "${bundle_search_dirs}")
    ]])
else()
    install(TARGETS DMirror RUNTIME DESTINATION bin)
    install(FILES README.md DESTINATION share/doc/dmirror)
endif()

include(CPack)
