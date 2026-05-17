cmake_minimum_required(VERSION 3.22)

project(jsonata
    VERSION 0.1.1
    DESCRIPTION "QJSONata for C++"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)
include(FetchContent)

set( JSONATA_ENABLED_BACKENDS )

# Find nlohmann_json or fetch if not found
find_package(nlohmann_json CONFIG QUIET)
if(NOT nlohmann_json_FOUND)
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.12.0
    )
    FetchContent_MakeAvailable(nlohmann_json)
    # Mark that we used FetchContent
    set(JSONATA_USED_FETCHCONTENT_JSON TRUE)
    list( APPEND JSONATA_ENABLED_BACKENDS nlohmann::ordered_json nlohmann::json )
    set( JSONATA_NLOHMANN_ENABLED true )
endif()

find_package(Qt6 QUIET COMPONENTS Core)
if(Qt6_FOUND)
    list( APPEND JSONATA_ENABLED_BACKENDS QJsonValue QVariant )
   set( JSONATA_QT_ENABLED true )
else()
    warning( "Cannot find Qt >=6.10." )
endif()

# Collect source files
file(GLOB_RECURSE HEADERS "include/*.h")
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Library type option
option(JSONATA_BUILD_SHARED "Build qjsonata as a shared library" OFF)
set(JSONATA_LIB_TYPE STATIC)
if(JSONATA_BUILD_SHARED)
    set(JSONATA_LIB_TYPE SHARED)
endif()

# Create the library
add_library(qjsonata ${JSONATA_LIB_TYPE} ${SOURCES} ${HEADERS})

set_target_properties(qjsonata PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    POSITION_INDEPENDENT_CODE ON
)

# Provide a canonical alias for consumers who add_subdirectory()
add_library(qjsonata::qjsonata ALIAS qjsonata)

# Compiler features
# target_compile_features(qjsonata PUBLIC cxx_std_17)

# Compiler options
target_compile_options(qjsonata
    PRIVATE
    # Common warnings
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wno-unused-variable>
    # Clang-only suppression
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wno-unused-lambda-capture>
    # MSVC: ensure source files are treated as UTF-8 to avoid codepage warnings
    $<$<CXX_COMPILER_ID:MSVC>:/utf-8>
)

# Set include directories for the library
target_include_directories(qjsonata PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/utfcpp>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if( JSONATA_NLOHMANN_ENABLED )
    # nlohmann_json is used from public headers, so propagate it PUBLIC.
    # Ensure the exported target only records the imported name in the install
    # interface to avoid referencing non-exported build targets.
    if(TARGET nlohmann_json::nlohmann_json)
        target_link_libraries(qjsonata PUBLIC
            $<BUILD_INTERFACE:nlohmann_json::nlohmann_json>
            $<INSTALL_INTERFACE:nlohmann_json::nlohmann_json>
        )
    elseif(TARGET nlohmann_json)
        target_link_libraries(qjsonata PUBLIC
            $<BUILD_INTERFACE:nlohmann_json>
            $<INSTALL_INTERFACE:nlohmann_json::nlohmann_json>
        )
    else()
        # Not present in the build; rely on the installed config to find it.
        target_link_libraries(qjsonata PUBLIC
            $<INSTALL_INTERFACE:nlohmann_json::nlohmann_json>
        )
    endif()
endif()

if( JSONATA_QT_ENABLED )
    target_link_libraries(qjsonata PUBLIC Qt6::Core)
endif()

# Optionally build tests
option(JSONATA_BUILD_TESTS "Build QJSONata C++ tests" OFF)

if(JSONATA_BUILD_TESTS)
    # for testing at least one backend needs to be enabled

    # 1. Check the length of the list
    list(LENGTH JSONATA_ENABLED_BACKENDS LIST_LEN)

    # 2. If the length is 0, throw a fatal error
    if(LIST_LEN EQUAL 0)
        message(FATAL_ERROR "At least one backend must be enabled.")
    endif()

    # Download GoogleTest if not already present
    # See https://google.github.io/googletest/quickstart-cmake.html
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP true
    )
    FetchContent_MakeAvailable(googletest)

    # Use CMake Threads package for cross-platform thread linkage
    find_package(Threads REQUIRED)

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

    # Create test generator executable
    add_executable(qgenerate test/Generate.cpp
        include/jsonata/backend.h)
    target_link_libraries(qgenerate qjsonata)
    list(GET JSONATA_ENABLED_BACKENDS 0 FIRST_BACKEND)
    target_compile_definitions(qgenerate PRIVATE
        JSONATA_TEST_BACKEND=${FIRST_BACKEND}
    )

    if(WIN32)
        set_target_properties(qgenerate PROPERTIES
            WIN32_EXECUTABLE FALSE
        )
    endif()

    foreach( BE IN LISTS JSONATA_ENABLED_BACKENDS )
        # 1. Strip the extension to create a clean executable name
        string(REPLACE "::" "_" CLEAN_BE ${BE})
        set(EXE_NAME qjsonata_tests_${CLEAN_BE})

        # 2. Create the executable
        add_executable(
            ${EXE_NAME}
            ${TEST_SOURCES}
        )

        target_compile_definitions(${EXE_NAME} PRIVATE
            JSONATA_TEST_BACKEND=${BE}
        )

        target_link_libraries(
            ${EXE_NAME}
            qjsonata
            gtest_main
            gtest
            Threads::Threads
        )
        if(WIN32)
            set_target_properties(${EXE_NAME} PROPERTIES
                WIN32_EXECUTABLE FALSE
            )
        endif()

    endforeach()


    # Custom command to generate test files
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/test/gen/generated.stamp
        COMMAND $<TARGET_FILE:qgenerate>
        COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_SOURCE_DIR}/test/gen/generated.stamp
        DEPENDS qgenerate
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating QJSONata test files from test suite"
    )

    # Custom target that depends on the generated files
    add_custom_target(qgenerate_tests
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test/gen/generated.stamp
    )

    foreach( BE IN LISTS JSONATA_ENABLED_BACKENDS )
        # 1. Strip the extension to create a clean executable name
        string(REPLACE "::" "_" CLEAN_BE ${BE})
        set(EXE_NAME qjsonata_tests_${CLEAN_BE})
        # Make sure generated tests are created before building main tests
        add_dependencies(${EXE_NAME} qgenerate_tests)
    endforeach()

    # Convenience target to just run the generator
    add_custom_target(run_generator
        COMMAND $<TARGET_FILE:qgenerate>
        DEPENDS qgenerate
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running QJSONata test generator"
    )

    # Option to build generated tests as separate executables
    option(BUILD_GENERATED_TESTS "Build generated QJSONata test suites" ON)

    if(BUILD_GENERATED_TESTS)
        foreach( BE IN LISTS JSONATA_ENABLED_BACKENDS )
            # 1. Strip the extension to create a clean executable name
            string(REPLACE "::" "_" CLEAN_BE ${BE})
            set(EXE_NAME qjsonata_generated_tests_${CLEAN_BE})

            # Find all generated test files
            file(GLOB GENERATED_TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test/gen/*Test.cpp")
        
            if(GENERATED_TEST_FILES)
                # Create executable for generated tests
                add_executable(${EXE_NAME} ${GENERATED_TEST_FILES} test/JsonataTest.cpp)
                target_compile_definitions(${EXE_NAME} PRIVATE
                    JSONATA_TEST_BACKEND=${BE}
                )
                target_include_directories(${EXE_NAME} PRIVATE test)
                target_link_libraries(${EXE_NAME}
                    qjsonata
                    gtest_main
                    gtest
                    Threads::Threads
                )
                # Windows-only: increase stack size to 8MB for generated tests
                if(WIN32)
                    if(MSVC)
                        target_link_options(${EXE_NAME} PRIVATE /STACK:8388608)
                    else()
                        target_link_options(${EXE_NAME} PRIVATE -Wl,--stack,8388608)
                    endif()
                endif()
                add_dependencies(${EXE_NAME} qgenerate_tests)

                # Add to testing (only if Google Test is available)
                if(TARGET gtest)
                    include(GoogleTest)
                    gtest_discover_tests(${EXE_NAME}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    )
                endif()
            endif()
        endforeach()
    endif()
endif()

# Installation rules
if(JSONATA_USED_FETCHCONTENT_JSON)
    if( JSONATA_NLOHMANN_ENABLED )
        install(TARGETS qjsonata nlohmann_json
            EXPORT qjsonataTargets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )
    endif()
else()
    install(TARGETS qjsonata
        EXPORT qjsonataTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY third_party/utfcpp/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export targets for the install-tree
install(EXPORT qjsonataTargets
    FILE jsonataTargets.cmake
    NAMESPACE qjsonata::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/qjsonata
)

# Generate and install package config + version files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/qjsonataConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    cmake/jsonataConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/qjsonataConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/qjsonata
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/qjsonataConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/qjsonataConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/qjsonata
)

# Enable testing with Google Test
if(JSONATA_BUILD_TESTS)
    foreach( BE IN LISTS JSONATA_ENABLED_BACKENDS )
        # 1. Strip the extension to create a clean executable name
        string(REPLACE "::" "_" CLEAN_BE ${BE})
        set(EXE_NAME qjsonata_tests_${CLEAN_BE})
        enable_testing()
        include(GoogleTest)
        gtest_discover_tests(${EXE_NAME}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endforeach()
endif()
