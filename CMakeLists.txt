cmake_minimum_required(VERSION 3.16)

project(OpenglEngine VERSION 0.1 LANGUAGES CXX)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Od")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG")
if (MSVC)
    add_compile_options(/utf-8)
endif()

# ����������
set(LibDir   ./DependLib/assimp/lib   ./DependLib/spdlog/lib )
# ��������ͷ�ļ�
set(IncludeDir  ./DependLib/glm 
./DependLib/assimp/include ./DependLib/spdlog/include )

# ���ؿ�·��
link_directories(${LibDir})
# ����ͷ�ļ�·��
include_directories(${IncludeDir})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets  LinguistTools )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets LinguistTools )



# # ���� .cpp
 file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*.cpp)
 file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS src/*.h)
 file(GLOB_RECURSE UI_FILES CONFIGURE_DEPENDS src/*.ui)
 file(GLOB_RECURSE SHADER_FILES src/*.glsl src/*.frag src/*.vert src/*.geom src/*.vs src/*.fs)
 set_source_files_properties(${SHADER_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
 qt_wrap_ui(OpenglEngine UI_HEADERS  ${UI_FILES} )

if (MSVC AND CMAKE_GENERATOR MATCHES "^Ninja")
    # Localized /showIncludes output may not be parsed by Ninja. Conservatively
    # rebuild project sources when project headers or UI definitions change.
    set_property(SOURCE ${SOURCE_FILES} main.cpp APPEND PROPERTY
        OBJECT_DEPENDS ${HEADER_FILES} ${UI_FILES})
endif()

set(PROJECT_SOURCES  
        main.cpp
        ${HEADER_FILES} ${SOURCE_FILES} ${UI_FILES} ${SHADER_FILES}
)


set(LIBS    assimp-vc142-mt)
qt_add_executable(OpenglEngine ${PROJECT_SOURCES}
    src/OpenglWidget/LightEditWidget.h src/OpenglWidget/LightEditWidget.cpp src/OpenglWidget/LightEditWidget.ui)



set_target_properties(${PROJECT_NAME}
    PROPERTIES
        WIN32_EXECUTABLE false
)
target_link_libraries(OpenglEngine PRIVATE  ${LIBS}  Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGLWidgets )
target_include_directories(OpenglEngine PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set(TS_FILES
    translations/zh_CN.ts
    translations/en_US.ts
)
# ���� �Լ�����ts qm �ļ�
set(QM_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/translations")
qt_add_translations(OpenglEngine
    TS_FILES 
        translations/zh_CN.ts
        translations/en_US.ts
     QM_OUTPUT_DIRECTORY ${QM_OUTPUT_PATH}
)
# �ؼ�������������ȷ��������Ŀ��ǰ���·���
add_dependencies(OpenglEngine update_translations)
