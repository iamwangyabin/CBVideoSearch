cmake_minimum_required ( VERSION 3.0)
set(CMAKE_CXX_COMPILER      "clang++" )         # 显示指定使用的C++编译器

set(CMAKE_CXX_FLAGS   "-std=c++11")             # c++11
set(CMAKE_CXX_FLAGS   "-g")                     # 调试信息
set(CMAKE_CXX_FLAGS   "-Wall")                  # 开启所有警告
project(hello)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src ${hello_src})
target_link_libraries(${PROJECT_NAME} util)
add_executable(${PROJECT_NAME} ${hello_src})

#g++ -c .\loadFile.cpp -o .\loadFile.o -I ..\include\
#g++ DictionaryBuilder.cpp -o main -I ..\include\  .\loadFile.o

#g++ .\BoWBuilder.cpp -o BoWmain -I ..\include\ .\loadFile.o