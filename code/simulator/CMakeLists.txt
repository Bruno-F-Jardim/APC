cmake_minimum_required(VERSION 3.18)
project(MyLittleBigUniverse LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

find_package(OpenMP REQUIRED)

# ==========================================================
# Core Simulation Library
# ==========================================================
add_library(core
    src/core/Body.cpp
    src/core/PhysicsEngine.cpp
    src/core/Universe.cpp
)
target_include_directories(core PUBLIC src)

# Link OpenMP and add optimization flags (O3 and native architecture)
target_link_libraries(core PRIVATE OpenMP::OpenMP_CXX)
target_compile_options(core PRIVATE -O3 -march=native)

# ==========================================================
# Executable
# ==========================================================
add_executable(simulate
    src/app/simulate.cpp
    src/app/initialConditions.cpp
    src/io/outputWritter.cpp

)
target_link_libraries(simulate PRIVATE core)

# ==========================================================
# Global include directories
# ==========================================================
include_directories(src)
