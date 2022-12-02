cmake_minimum_required (VERSION 3.8)

include_directories("${CMAKE_SOURCE_DIR}/external/fmt/include")
include_directories("${CMAKE_SOURCE_DIR}/common")

add_executable (AdventOfCode2022_DayXX "solution.cpp")

target_link_libraries(AdventOfCode2022_DayXX PRIVATE AoC_Common)
target_link_libraries(AdventOfCode2022_DayXX PRIVATE fmt::fmt-header-only)

add_custom_command(TARGET AdventOfCode2022_DayXX POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                           ${CMAKE_CURRENT_SOURCE_DIR}/input.txt
                           ${CMAKE_CURRENT_BINARY_DIR}/input.txt)

set_property(TARGET AdventOfCode2022_DayXX PROPERTY CXX_STANDARD 17)