cmake_minimum_required(VERSION 3.0)#版本号
project(huanghao_code)#项目名称

aux_source_directory(./src SRC_SUB) #子目录中的源文件，赋值给SRC_SUB
aux_source_directory(. SRC_CUR) #当前目录下的源文件，赋值给SRC_CUR

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")#编译时显示警告，选择g++编译器
set(CMAKE_BUILD_TYPE Debug)	#可执行文件设为可调试版本， 默认是Release模式

include_directories(${CMAKE_SOURCE_DIR}/include)#头文件路径，CMAKE_SOURCE_DIR
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}) # 设置可执行文件的输出目录

add_executable(a ${SRC_SUB} ${SRC_CUR})#生成可执行文件