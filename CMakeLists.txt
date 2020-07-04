cmake_minimum_required(VERSION 3.5)

project(glimpse
        VERSION 0.1.0
        LANGUAGES CXX
        DESCRIPTION "Small OpenGL wrapper for modern C++")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG)

## Target ##
set(GLIMPSE_H
        include/glimpse/gl.hpp
        include/glimpse/types.hpp
        include/glimpse/error.hpp
        include/glimpse/image_format.hpp
        include/glimpse/buffer_format.hpp
        include/glimpse/buffer.hpp
        include/glimpse/framebuffer.hpp
        include/glimpse/renderbuffer.hpp
        include/glimpse/program.hpp
        include/glimpse/texture.hpp
        include/glimpse/texture_cube.hpp
        include/glimpse/texture_3d.hpp
        include/glimpse/texture_array.hpp
        include/glimpse/vertex_array.hpp
        include/glimpse/data.hpp
        include/glimpse/member.hpp
        include/glimpse/attribute.hpp
        include/glimpse/uniform.hpp)
set(GLIMPSE_CXX_FLAGS
        $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:
        $<$<CONFIG:Debug>:-Wall -Wextra>>
        $<$<C_COMPILER_ID:MSVC>:
        $<$<CONFIG:Debug>:/Wall /Zi>>)

add_library(glimpse
        ${GLIMPSE_H}
        src/error.cpp
        src/types.cpp
        src/image_format.cpp
        src/buffer.cpp
        src/framebuffer.cpp
        src/renderbuffer.cpp
        src/program.cpp
        src/texture.cpp
        src/texture_cube.cpp
        src/texture_3d.cpp
        src/texture_array.cpp
        src/vertex_array.cpp
        src/member.cpp
        src/attribute.cpp
        src/uniform.cpp)
add_library(glimpse::glimpse ALIAS glimpse)
set_target_properties(glimpse
        PROPERTIES
        VERSION "${PROJECT_VERSION}"
        PUBLIC_HEADER "${GLIMPSE_H}")
target_include_directories(glimpse
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
        $<INSTALL_INTERFACE:include/>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_options(glimpse PRIVATE ${GLIMPSE_CXX_FLAGS})
target_link_libraries(glimpse PRIVATE
        OpenGL::GL
        GLEW::GLEW
        glm)

## Static Analysis ##
option(ENABLE_CPPCHECK "Enable static analysis with cppcheck" OFF)
option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)

if (ENABLE_CPPCHECK)
    find_program(CPPCHECK cppcheck)
    if (CPPCHECK)
        set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --suppress=missingInclude --enable=all
                --inconclusive -i ${CMAKE_SOURCE_DIR}/imgui/lib)
    else ()
        message(SEND_ERROR "cppcheck requested but executable not found")
    endif ()
endif ()

if (ENABLE_CLANG_TIDY)
    find_program(CLANGTIDY clang-tidy)
    if (CLANGTIDY)
        set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY})
    else ()
        message(SEND_ERROR "clang-tidy requested but executable not found")
    endif ()
endif ()

## Installation ##
include(GNUInstallDirs)

install(TARGETS glimpse
        EXPORT GlimpseConfig
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${LIBLEGACY_INCLUDE_DIRS}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")
install(EXPORT GlimpseConfig
        NAMESPACE glimpse::
        DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake")
export(EXPORT GlimpseConfig)
export(PACKAGE glimpse)


