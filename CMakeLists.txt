cmake_minimum_required(VERSION 3.10)

project(OpenglEngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)




#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Od")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG")
add_compile_options(/utf-8)

# 添加依赖库
set(LibDir   ./DependLib/assimp/lib   ./DependLib/spdlog/lib )
# 添加依赖头文件
set(IncludeDir  ./DependLib/glm 
./DependLib/assimp/include ./DependLib/spdlog/include )

# 加载库路径
link_directories(${LibDir})
# 加载头文件路径
include_directories(${IncludeDir})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# # 添加 .cpp
 file(GLOB_RECURSE SOURCE_FILES  src/*.cpp)
 file(GLOB_RECURSE HEADER_FILES  src/*.h)
 file(GLOB_RECURSE UI_FILES   src/*.ui)
 qt_wrap_ui(UI_HEADERS  ${UI_FILES} )

set(PROJECT_SOURCES  
        main.cpp
        ${HEADER_FILES} ${SOURCE_FILES} ${UI_FILES}
)

set(LIBS    assimp-vc142-mt)
add_executable(OpenglEngine ${PROJECT_SOURCES})
target_link_libraries(OpenglEngine PRIVATE  ${LIBS}  Qt${QT_VERSION_MAJOR}::Widgets )
target_include_directories(OpenglEngine PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
message(STATUS "UI files: ${PROJECT_SOURCES}")

 