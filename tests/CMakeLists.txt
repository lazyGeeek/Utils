include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

string(TOLOWER "${CMAKE_SYSTEM_NAME}" UTILS_OS)
string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" UTILS_ARCH)

set(UTILS_TESTS_OUTPUT "${CMAKE_SOURCE_DIR}/bin/${UTILS_ARCH}/${UTILS_OS}/Tests")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${UTILS_TESTS_OUTPUT}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${UTILS_TESTS_OUTPUT}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${UTILS_TESTS_OUTPUT}")

file(GLOB_RECURSE UTILS_TEST_SOURCES CONFIGURE_DEPENDS
    LIST_DIRECTORIES false
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(utils_tests ${UTILS_TEST_SOURCES})

target_link_libraries(utils_tests PRIVATE GTest::gtest_main Utils)

target_include_directories(utils_tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/support"
    "${CMAKE_SOURCE_DIR}/include"
)

add_custom_command(TARGET utils_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:Utils>"
        "$<TARGET_FILE_DIR:utils_tests>"
    COMMENT "Copying Utils runtime library to Tests output"
)

include(GoogleTest)
gtest_discover_tests(utils_tests WORKING_DIRECTORY "${UTILS_TESTS_OUTPUT}")
