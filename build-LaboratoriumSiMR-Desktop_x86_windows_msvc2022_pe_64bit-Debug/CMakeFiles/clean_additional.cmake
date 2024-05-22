# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\LaboratoriumSiMR_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\LaboratoriumSiMR_autogen.dir\\ParseCache.txt"
  "LaboratoriumSiMR_autogen"
  )
endif()
