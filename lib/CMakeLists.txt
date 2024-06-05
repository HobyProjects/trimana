set(SDL_SHARED ON)
set(SDL_STATIC OFF)
add_subdirectory("SDL" EXCLUDE_FROM_ALL)
add_subdirectory("spdlog" EXCLUDE_FROM_ALL)

set(GLM_BUILD_TESTS ON)
add_subdirectory("glm" EXCLUDE_FROM_ALL)

set(glew-cmake_BUILD_STATIC ON)
set(glew-cmake_BUILD_SHARED OFF)
set(ONLY_LIBS ON)
add_subdirectory("glew-cmake" EXCLUDE_FROM_ALL)
add_subdirectory("assimp" EXCLUDE_FROM_ALL)