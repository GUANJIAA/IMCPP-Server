# 设置cmake的最低版本和项目名称
cmake_minimum_required(VERSION 2.0)
project(IMCPP)

# 生成debug版本，可以进行gdb调试
set(CMAKE_BUILD_TYPE "Debug")

# 设置项目可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 设置项目库文件输出的路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 设置项目编译头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/src/rpc/include)
include_directories(${PROJECT_SOURCE_DIR}/src/redispb)
include_directories(${PROJECT_SOURCE_DIR}/src/proto)
include_directories(${PROJECT_SOURCE_DIR}/src/mysqldb)
include_directories(${PROJECT_SOURCE_DIR}/src/model)
include_directories(${PROJECT_SOURCE_DIR}/src/redisdb)
# 设置项目库文件搜索路径
link_directories(${PROJECT_SOURCE_DIR}/lib)
link_directories(/usr/local/lib)
link_directories(/usr/local/lib64)

# src包含了mprpc框架所有的相关代码
add_subdirectory(src)