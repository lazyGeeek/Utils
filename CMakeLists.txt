cmake_minimum_required(VERSION 3.25)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
 
project(Utils LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directory: bin/<architecture>/<os>/<Debug|Release>
string(TOLOWER "${CMAKE_SYSTEM_NAME}" UTILS_OS)
string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" UTILS_ARCH)
set(UTILS_BIN_ROOT "${CMAKE_SOURCE_DIR}/bin/${UTILS_ARCH}/${UTILS_OS}")

if(CMAKE_CONFIGURATION_TYPES)
    # Multi-config generators (Visual Studio, Ninja Multi-Config).
    foreach(cfg IN LISTS CMAKE_CONFIGURATION_TYPES)
        string(TOUPPER "${cfg}" cfg_upper)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfg_upper} "${UTILS_BIN_ROOT}/${cfg}")
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${cfg_upper} "${UTILS_BIN_ROOT}/${cfg}")
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${cfg_upper} "${UTILS_BIN_ROOT}/${cfg}")
    endforeach()
    set(UTILS_OUTPUT_DIR "${UTILS_BIN_ROOT}/$<CONFIG>")
else()
    # Single-config generators.
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    endif()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${UTILS_BIN_ROOT}/${CMAKE_BUILD_TYPE}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${UTILS_BIN_ROOT}/${CMAKE_BUILD_TYPE}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${UTILS_BIN_ROOT}/${CMAKE_BUILD_TYPE}")
    set(UTILS_OUTPUT_DIR "${UTILS_BIN_ROOT}/${CMAKE_BUILD_TYPE}")
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "" FORCE)

file(GLOB_RECURSE UTILS_SOURCES CONFIGURE_DEPENDS
    LIST_DIRECTORIES false
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

file(GLOB_RECURSE UTILS_HEADERS CONFIGURE_DEPENDS
    LIST_DIRECTORIES false
    "${CMAKE_CURRENT_SOURCE_DIR}/include/utils/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/utils/*.inl")

add_library(Utils SHARED ${UTILS_SOURCES} ${UTILS_HEADERS})

target_include_directories(Utils
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)

foreach(_source IN ITEMS ${UTILS_SOURCES} ${UTILS_HEADERS})
    get_filename_component(_path "${_source}" PATH)
    file(RELATIVE_PATH _rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_path}")
    string(REPLACE "\\" "/" _group "${_rel}")
    source_group("${_group}" FILES "${_source}")
endforeach()

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
