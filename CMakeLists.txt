cmake_minimum_required(VERSION 3.15...3.27)
project(linalg)
set(CMAKE_CXX_STANDARD 23)


# Set GCC as the compiler if it's not already set
if (APPLE)
  set(CMAKE_C_COMPILER /opt/homebrew/bin/gcc-14)
  set(CMAKE_CXX_COMPILER /opt/homebrew/bin/g++-14)
endif()

if (CMAKE_VERSION VERSION_LESS 3.18)
  set(DEV_MODULE Development)
else()
  set(DEV_MODULE Development.Module)
endif()


find_package(Python 3.8 COMPONENTS Interpreter ${DEV_MODULE} REQUIRED)

find_package(OpenMP REQUIRED)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ext/nanobind)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ext/annoy)


include_directories("include/")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

nanobind_add_module(linalg src/bindings.cpp)

add_executable(test_fastmap src/test_fastmap.cpp)

# Link OpenMP
if(OpenMP_CXX_FOUND)
  target_link_libraries(linalg PUBLIC OpenMP::OpenMP_CXX)
endif()



