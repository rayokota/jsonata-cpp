cmake_minimum_required(VERSION 3.22)

project(jsonata
    VERSION 1.0.0 
    DESCRIPTION "JSONata for C++"
    LANGUAGES CXX
)

include(GNUInstallDirs)
include(FetchContent)

# Find nlohmann_json or fetch if not found
find_package(nlohmann_json QUIET)
if(NOT nlohmann_json_FOUND)
    FetchContent_Declare(
            nlohmann_json
            GIT_REPOSITORY https://github.com/nlohmann/json.git
            GIT_TAG        v3.12.0
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif()

# Collect source files
file(GLOB_RECURSE HEADERS "include/*.h")
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Library type option
option(JSONATA_BUILD_SHARED "Build jsonata as a shared library" OFF)
set(JSONATA_LIB_TYPE STATIC)
if(JSONATA_BUILD_SHARED)
    set(JSONATA_LIB_TYPE SHARED)
endif()

# Create the library
add_library(jsonata ${JSONATA_LIB_TYPE} ${SOURCES} ${HEADERS})

set_target_properties(jsonata PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    POSITION_INDEPENDENT_CODE ON
)

# Provide a canonical alias for consumers who add_subdirectory()
add_library(jsonata::jsonata ALIAS jsonata)

# Compiler features
target_compile_features(jsonata PUBLIC cxx_std_17)

# Compiler options
target_compile_options(jsonata
        PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -Wall -Wno-unused-variable -Wno-unused-lambda-capture>
)

# Set include directories for the library
target_include_directories(jsonata PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/utfcpp>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Link dependency handling for nlohmann_json
# When using find_package, we can link publicly to the imported target
# When using FetchContent, we need to handle it differently to avoid export issues
if(nlohmann_json_FOUND)
    # Found via find_package - can link publicly
    target_link_libraries(jsonata PUBLIC nlohmann_json::nlohmann_json)
else()
    # Fetched via FetchContent - link privately and let consumers find their own
    if(TARGET nlohmann_json::nlohmann_json)
        target_link_libraries(jsonata PRIVATE nlohmann_json::nlohmann_json)
    elseif(TARGET nlohmann_json)
        target_link_libraries(jsonata PRIVATE nlohmann_json)
    endif()
endif()

if(_JSONATA_NLOHMANN_BUILD_TARGET)
    target_link_libraries(jsonata PUBLIC
        $<BUILD_INTERFACE:${_JSONATA_NLOHMANN_BUILD_TARGET}>
        $<INSTALL_INTERFACE:nlohmann_json::nlohmann_json>
    )
else()
    target_link_libraries(jsonata PUBLIC
        $<INSTALL_INTERFACE:nlohmann_json::nlohmann_json>
    )
endif()

# Optionally build tests
option(JSONATA_BUILD_TESTS "Build JSONata C++ tests" ON)

if(JSONATA_BUILD_TESTS)
    # Download GoogleTest if not already present
    # See https://google.github.io/googletest/quickstart-cmake.html
    FetchContent_Declare(
            googletest
            URL                    https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
            DOWNLOAD_EXTRACT_TIMESTAMP true
    )
    FetchContent_MakeAvailable(googletest)

    # Collect all Google Test format test files
    set(TEST_SOURCES
        test/ArrayTest.cpp
        test/CustomFunctionTest.cpp
        test/DateTimeTest.cpp
        test/NullSafetyTest.cpp
        test/NumberTestSimplified.cpp
        test/ParseIntegerTest.cpp
        test/SerializationTest.cpp
        test/SignatureTest.cpp
        test/StringTest.cpp
        test/ThreadTest.cpp
        test/TypesTest.cpp
    )

    # Create main test executable
    add_executable(
      jsonata_tests
      ${TEST_SOURCES}
    )

    target_link_libraries(
      jsonata_tests
      jsonata
      gtest_main
      gtest
      pthread
    )

    # Create test generator executable
    add_executable(generate test/Generate.cpp)
    target_link_libraries(generate jsonata)

    # Custom command to generate test files
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/test/gen/generated.stamp
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/generate
        COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_SOURCE_DIR}/test/gen/generated.stamp
        DEPENDS generate
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating JSONata test files from test suite"
    )

    # Custom target that depends on the generated files
    add_custom_target(generate_tests 
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test/gen/generated.stamp
    )

    # Make sure generated tests are created before building main tests
    add_dependencies(jsonata_tests generate_tests)

    # Convenience target to just run the generator
    add_custom_target(run_generator
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/generate
        DEPENDS generate
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running JSONata test generator"
    )

    # Option to build generated tests as separate executables
    option(BUILD_GENERATED_TESTS "Build generated JSONata test suites" ON)

    if(BUILD_GENERATED_TESTS)
        # Find all generated test files
        file(GLOB GENERATED_TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test/gen/*Test.cpp")
        
        if(GENERATED_TEST_FILES)
            # Create executable for generated tests
            add_executable(jsonata_generated_tests ${GENERATED_TEST_FILES} test/JsonataTest.cpp)
            target_include_directories(jsonata_generated_tests PRIVATE test)
            target_link_libraries(jsonata_generated_tests 
                jsonata
                gtest_main 
                gtest 
                pthread
            )
            add_dependencies(jsonata_generated_tests generate_tests)
            
            # Add to testing (only if Google Test is available)
            if(TARGET gtest)
                include(GoogleTest)
                gtest_discover_tests(jsonata_generated_tests)
            endif()
        endif()
    endif()
endif()

# Installation rules
install(TARGETS jsonata
    EXPORT jsonataTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY third_party/utfcpp/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export targets for the install-tree
install(EXPORT jsonataTargets
    FILE jsonataTargets.cmake
    NAMESPACE jsonata::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jsonata
)

# Generate and install package config + version files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/jsonataConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    cmake/jsonataConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/jsonataConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jsonata
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/jsonataConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/jsonataConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jsonata
)

# Enable testing with Google Test
if(JSONATA_BUILD_TESTS)
    enable_testing()
    include(GoogleTest)
    gtest_discover_tests(jsonata_tests)
endif()
