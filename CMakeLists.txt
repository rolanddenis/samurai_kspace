project(SamuraiKSpace)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

add_compile_options(-Wall -Wextra -pedantic)
add_compile_options($<$<CONFIG:Release>:-march=native>)
add_compile_options($<$<CONFIG:Release>:-mtune=native>)

include_directories(${PROJECT_SOURCE_DIR}/include)

OPTION(BUILD_EXAMPLES "Build examples." ON)
if (BUILD_EXAMPLES)
  add_subdirectory(${PROJECT_SOURCE_DIR}/examples)
endif (BUILD_EXAMPLES)

OPTION(BUILD_TESTING "Build tests." ON)
if (BUILD_TESTING)
  enable_testing()
  add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif (BUILD_TESTING)

