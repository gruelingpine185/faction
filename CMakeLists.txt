cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(faction C)

set(CMAKE_C_STANDARD 11)

# paths
set(inc_dir ${CMAKE_SOURCE_DIR}/inc/faction)
set(src_dir ${CMAKE_SOURCE_DIR}/src)
set(bin_dir ${CMAKE_SOURCE_DIR}/bin)
set(vnd_dir ${CMAKE_SOURCE_DIR}/vendor)
set(code_dirs renderer utils)

# files
file(GLOB_RECURSE c_headers ${inc_dir}/*.h)
file(GLOB_RECURSE c_sources ${src_dir}/*.c)
list(FILTER c_sources EXCLUDE REGEX ${src_dir}/main.c)

# Compiler flags
set(c_std "-std=c11")
set(c_opt "-O2")
set(c_wrn "-Wall -Wextra -pedantic")
set(c_def "-DF_DEBUG_MODE=1")
set(c_inc "-I${inc_dir} -I${vnd_dir}")

set(CMAKE_C_FLAGS "${c_std} ${c_opt} ${c_wrn} ${c_def} ${c_inc}")

# vendor
# --------------------------#

# GLFW
add_subdirectory(${vnd_dir}/glfw)

# Vulkan
find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

# volk
include_directories(${vnd_dir})

# --------------------------#

# target
add_executable(faction ${c_sources} ${c_headers} ${src_dir}/main.c ${vnd_dir}/volk/volk.c)
target_link_libraries(faction glfw)

# bin dir
set_target_properties(faction PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_dir})

# bin dirs
foreach(path ${code_dirs})
    file(MAKE_DIRECTORY ${bin_dir}/${path})
endforeach()
