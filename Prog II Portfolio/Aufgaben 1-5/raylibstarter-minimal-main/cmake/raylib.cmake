# Based on: https://raw.githubusercontent.com/raysan5/raylib/master/projects/CMake/CMakeLists.txt

# Set this to the minimal version you want to support
find_package(raylib 4.0.0 QUIET) # Let CMake search for a raylib-config.cmake

# You could change the QUIET above to REQUIRED and remove this if() clause
# This part downloads raylib and builds it if it's not installed on your system
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)

    FetchContent_Declare(
            raylib
            # Download a specific release
            URL https://github.com/raysan5/raylib/archive/refs/tags/4.0.0.tar.gz
            # Or download current working state
            # URL https://github.com/raysan5/raylib/archive/master.tar.gz
    )

    FetchContent_MakeAvailable(raylib)

endif()
