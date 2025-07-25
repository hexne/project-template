cmake_minimum_required(VERSION 4.0.0)

set(CMAKE_CXX_STANDARD 26)
# 根据cmake版本查找对应uuid https://github.com/Kitware/CMake/blob/v4.0.3/Help/dev/experimental.rst
# 4.0.2
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "a9e1cf81-9932-4810-974b-6eccaf14e457")
# 4.0.3
# set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
set(CMAKE_CXX_MODULE_STD 1)

project(project-template)

add_compile_options(--embed-dir=${CMAKE_CURRENT_SOURCE_DIR}/res) # ok


# ===========================================
# C++模块
# ===========================================
file(GLOB module_files "src/*.cppm")
add_library(module STATIC)
target_sources(module
        PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES FILES
        ${module_files}
)

# ===========================================
# 静态库
# ===========================================
# add_library(${PROJECT_NAME} STATIC)
# target_sources(${PROJECT_NAME} PRIVATE
#         src/lib.cpp
# )

# ===========================================
# 动态库
# ===========================================
# add_library(${PROJECT_NAME} SHARED)
# target_sources(${PROJECT_NAME} PRIVATE
#     src/lib.cpp
# )

# ===========================================
# 子项目
# ===========================================
# add_subdirectory("../modforge" modforge)

# ===========================================
# 可执行文件
# ===========================================
add_executable(${PROJECT_NAME}
        src/main.cpp
)

# ===========================================
# 链接
# ===========================================
target_link_libraries(${PROJECT_NAME}
        module
)
