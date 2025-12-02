cmake_minimum_required(VERSION 3.16)

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

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets  LinguistTools )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets LinguistTools )
set(TS_FILES
    translations/zh_CN.ts
    translations/en_US.ts
)


# # 添加 .cpp
 file(GLOB_RECURSE SOURCE_FILES  src/*.cpp)
 file(GLOB_RECURSE HEADER_FILES  src/*.h)
 file(GLOB_RECURSE UI_FILES   src/*.ui)
 qt_wrap_ui(OpenglEngine UI_HEADERS  ${UI_FILES} )

set(PROJECT_SOURCES  
        main.cpp
        ${HEADER_FILES} ${SOURCE_FILES} ${UI_FILES}
)


set(LIBS    assimp-vc142-mt)
qt_add_executable(OpenglEngine ${PROJECT_SOURCES})



set_target_properties(${PROJECT_NAME}
    PROPERTIES
        WIN32_EXECUTABLE false
)
target_link_libraries(OpenglEngine PRIVATE  ${LIBS}  Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGLWidgets )
target_include_directories(OpenglEngine PRIVATE ${CMAKE_CURRENT_BINARY_DIR})


# 生成 以及更新ts qm 文件
qt_add_translations(OpenglEngine
    TS_FILES 
        translations/zh_CN.ts
        translations/en_US.ts
     QM_OUTPUT_DIRECTORY translations
)
# 关键：添加依赖，确保构建主目标前更新翻译
add_dependencies(OpenglEngine update_translations)