PROJECT(RappCloud)

cmake_minimum_required(VERSION 2.8)

find_package(Rapp REQUIRED)
MESSAGE(STATUS ${RAPP_INCLUDE_DIRS})
include_directories(${RAPP_INCLUDE_DIRS})

# Find PThreads (needed by Boost)
find_package(Threads REQUIRED)
# Find Boost
find_package( Boost 1.49 COMPONENTS system thread random unit_test_framework program_options REQUIRED )

# Find OpenCV 2.0
#find_package( OpenCV REQUIRED )




ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK) 
set(CMAKE_COLOR_MAKEFILE on)
set(CMAKE_VERBOSE_MAKEFILE off)
# Get from arguments if Debug is requested
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_BUILD_TYPE Debug)
    message(STATUS "Building: ${CMAKE_BUILD_TYPE} - ommit flag to change to `Release`")
else()
    set(CMAKE_BUILD_TYPE Release)
    message(STATUS "Building: ${CMAKE_BUILD_TYPE} - add `-DCMAKE_BUILD_TYPE=Debug` to switch to `Debug`")
endif()
# Linux should be the default OS/Platform for RAPP::API
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # using GCC/Linux
    message (STATUS "GCC/Linux" )
    execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
        message(STATUS "COMPILER: g++ version >= 4.7 using C++11")  
    elseif ()
        message (FATAL_ERROR "g++ version >= 4.7 is required")
    endif()
    if (${STATIC_LIBSTD})
        set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=gnu++11 -Wall -fPIC -static-libstdc++")
    else()
        set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=gnu++11 -Wall -fPIC")
    endif()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    # TODO: can somebody with a NAO test this?
    if (${OPEN_NAO})
        set(CMAKE_EXE_LINKER_FLAGS "-lc++ -lcxxrt")
        set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=gnu++11 -Wall -fPIC -static-libstdc++ -m32")
    endif()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -mtune=native -march=native")
    set(BUILD_SHARED_LIBS OFF)
    message(STATUS "CXX Flags: " ${CMAKE_CXX_FLAGS})
    message(STATUS "CXX Linker: " ${CMAKE_EXE_LINKER_FLAGS})
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # using Clang
    message (STATUS "Clang/OSX" )
    execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (CLANG_VERSION_MAJOR VERSION_GREATER 3 OR CLANG_VERSION_MAJOR VERSION_EQUAL 3)
        message(STATUS "Info: Clang >= 3.0 using C++11")
    elseif ()
        message(FATAL_ERROR "Clang version >= 3.0 is required")
    endif()
        set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -Wall")
        message(STATUS "CXX Flags: " ${CMAKE_CXX_FLAGS})
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -mtune=native -march=native")
        set(BUILD_SHARED_LIBS OFF)
        message(STATUS "CXX Linker: " ${CMAKE_EXE_LINKER_FLAGS})
# Intel Compiler May run on Linux - Maybe we should support it
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    message (FATAL_ERROR "unsupported compiler")
# We cannot support Microsoft Visual Studio - We cannot support Win32 platforms
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    message (FATAL_ERROR "unsupported compiler")
endif()

set (INCLUDES includes)
include_directories(${INCLUDES})

add_subdirectory(${INCLUDES})

install (DIRECTORY ${INCLUDES}/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/ FILES_MATCHING PATTERN "*.hpp" PATTERN "*.ihh")


# Prepare the CMake config files
configure_file(cmake/RappCloudConfig.cmake.in "${PROJECT_BINARY_DIR}/RappCloudConfig.cmake" @ONLY)
configure_file(cmake/RappCloudConfigVersion.cmake.in "${PROJECT_BINARY_DIR}/RappCloudConfigVersion.cmake" @ONLY)
 
# Install the CMake config files
install(FILES
	"${PROJECT_BINARY_DIR}/RappCloudConfig.cmake"
    "${PROJECT_BINARY_DIR}/RappCloudConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/share/RappCloud"
)


add_subdirectory(implementations)
