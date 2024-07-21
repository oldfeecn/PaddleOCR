cmake_minimum_required(VERSION 3.14)
#set(CMAKE_TOOLCHAIN_FILE "C:/CUDA/vcpkg/scripts/buildsystems/vcpkg.cmake"
#  CACHE STRING "Vcpkg toolchain file")
set(CMAKE_COLOR_MAKEFILE ON)# 启用彩色输出
# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(DEMO_NAME "BitBlt")

project(${DEMO_NAME} LANGUAGES CXX)

# 指定头文件的目录
include_directories(${CMAKE_SOURCE_DIR}/BitBlt/include)
# 添加库
# 查找必要的Windows库
find_library(D3D11_LIB d3d11)
find_library(DXGI_LIB dxgi)
find_library(D3D_COMPILER_LIB d3dcompiler)

# 自动获取所有 .cpp 文件
file(GLOB SRC_DIR "src/*.cpp")

add_executable(${DEMO_NAME} 
    ${SRC_DIR}
    #src/main.cpp
) 
target_link_libraries(${DEMO_NAME} PUBLIC
        ${DEPS}
)
# 链接 dwmapi.lib
if (WIN32)
    target_link_libraries(${DEMO_NAME} PUBLIC dwmapi ${D3D11_LIB} ${DXGI_LIB} ${D3D_COMPILER_LIB})
endif()
###########################################################################