#
# This is a CMake makefile.  You can find the cmake utility and
# information about it at http://www.cmake.org
#

cmake_minimum_required(VERSION 3.4)
PROJECT(li180c)

set(OpenCV_DIR "/usr/local/opt/opencv3/share/OpenCV/")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

MACRO(add_test name)
   ADD_EXECUTABLE(${name} src/${name}.cpp)
   TARGET_LINK_LIBRARIES(${name} ${OpenCV_LIBS})
   set_property(TARGET ${name} PROPERTY CXX_STANDARD 11)
ENDMACRO()

add_test(li180)
