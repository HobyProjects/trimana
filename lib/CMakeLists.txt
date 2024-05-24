set(SDL_SHARED OFF)
set(SDL_STATIC ON)
add_subdirectory("SDL" EXCLUDE_FROM_ALL)
add_subdirectory("spdlog" EXCLUDE_FROM_ALL)

set(GLM_BUILD_TESTS OFF)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory("glm" EXCLUDE_FROM_ALL)

set(glew-cmake_BUILD_STATIC ON)
set(glew-cmake_BUILD_SHARED OFF)
set(ONLY_LIBS ON)
add_subdirectory("glew-cmake" EXCLUDE_FROM_ALL)
add_subdirectory("assimp" EXCLUDE_FROM_ALL)