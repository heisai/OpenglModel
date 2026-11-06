## 开发环境： VS Studio 2022 、Qt6.10、 C++20  

- Windows 下构建会将 Qt 运行库部署到构建输出目录，并将中文翻译生成到 `build/translations`；请运行 `build/OpenglEngine.exe`。

- 架构分层说明文档：`Doc/架构分层设计说明.md`

- __开发问题：在初始化 带有initializeOpenGLFunctions(); 的类的时候， 要在QOpenGLWidget 自定义类构造之后初始化。__

- __生成翻译文件:__

  ```cpp	
  # 生成 以及更新ts qm 文件
  qt_add_translations(OpenglEngine
      TS_FILES 
          translations/zh_CN.ts
          translations/en_US.ts
  )
  # 需要更新 TS 翻译源文件时，手动构建 update_translations 目标；普通构建不会改写 TS 文件
  ```


- __指定路径生成ui_*.h:___

  __qt_wrap_ui(UI_HEADERS  ${UI_FILES} ) 强制生成ui_h 文件，默认生成在自定义buildRoot 路径， target_include_directories(OpenglEngine PRIVATE ${CMAKE_CURRENT_BINARY_DIR}) 要将ui_*h目录包括进来__

  ```cpp
   qt_wrap_ui(OpenglEngine UI_HEADERS  ${UI_FILES} )
   target_include_directories(OpenglEngine PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
  ```

  

- __vs studio 设置生成命令行终端 便于log 调试：__

  ```cpp
  set_target_properties(${PROJECT_NAME}
      PROPERTIES
          WIN32_EXECUTABLE false
  )
   WIN32_EXECUTABLE true //则是隐藏
  
  ```

  
