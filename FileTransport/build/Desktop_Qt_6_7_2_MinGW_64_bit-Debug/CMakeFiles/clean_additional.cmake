# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appFileTransport_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appFileTransport_autogen.dir\\ParseCache.txt"
  "appFileTransport_autogen"
  )
endif()
