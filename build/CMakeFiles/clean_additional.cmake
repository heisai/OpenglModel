# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "RelWithDebInfo")
  file(REMOVE_RECURSE
  "CMakeFiles\\OpenglEngine_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\OpenglEngine_autogen.dir\\ParseCache.txt"
  "OpenglEngine_autogen"
  )
endif()
