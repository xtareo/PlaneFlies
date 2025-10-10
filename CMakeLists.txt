cmake_minimum_required(VERSION 3.12)
project(DinosaurRun)

# 设置C标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 设置编译器
set(CMAKE_C_COMPILER "D:/Language/C/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "D:/Language/C/mingw64/bin/g++.exe")

# 设置构建输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

file(GLOB SOURCES "src/*.c")

# 包含目录
include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

# 链接目录
link_directories(
    ${PROJECT_SOURCE_DIR}/lib
)

# 添加可执行文件
add_executable(PlaneFlies ${SOURCES})

# 链接库
target_link_libraries(PlaneFlies SDL3 SDL3_ttf SDL3_image)
