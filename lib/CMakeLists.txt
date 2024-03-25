set(SDL_SHARED OFF)
set(SDL_STATIC ON)
add_subdirectory("SDL" EXCLUDE_FROM_ALL)
add_subdirectory("spdlog")
