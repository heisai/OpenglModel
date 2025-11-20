## 开发环境： VS Studio 2022 、Qt5.15.2、 C++20  

-  开发问题：
- __开发问题：在初始化 带有initializeOpenGLFunctions(); 的类的时候， 要在QOpenGLWidget 自定义类构造之后初始化。__

- __开发问题: qt_wrap_ui(UI_HEADERS  ${UI_FILES} ) 强制生成ui_h 文件，默认生成在自定义buildRoot 路径， target_include_directories(OpenglEngine PRIVATE ${CMAKE_CURRENT_BINARY_DIR}) 要将ui_*h目录包括进来__
