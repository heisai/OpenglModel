## 开发环境： VS Studio 2022 、Qt6.10、 C++20  

- __开发问题：在初始化 带有initializeOpenGLFunctions(); 的类的时候， 要在QOpenGLWidget 自定义类构造之后初始化。__

- __开发问题:__  __qt_wrap_ui(UI_HEADERS  ${UI_FILES} ) 强制生成ui_h 文件，默认生成在自定义buildRoot 路径， target_include_directories(OpenglEngine PRIVATE ${CMAKE_CURRENT_BINARY_DIR}) 要将ui_*h目录包括进来__

- __生成翻译文件:__

  ```cpp	
  # 生成 以及更新ts qm 文件
  qt_add_translations(OpenglEngine
      TS_FILES 
          translations/zh_CN.ts
          translations/en_US.ts
  )
  # 关键：添加依赖，确保构建主目标前更新翻译
  add_dependencies(OpenglEngine update_translations)
  ```

  

