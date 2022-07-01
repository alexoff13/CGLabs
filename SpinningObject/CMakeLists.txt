cmake_minimum_required(VERSION 3.22)
project(SpinningObject)

#find_package(OpenGL REQUIRED)
#find_package(GLUT REQUIRED)
#set(CMAKE_CXX_STANDARD 23)
##set(OpenGlLinkers -lglut -lGLU -lGL)
#
#add_executable(SpinningObject main.cpp)
#target_link_libraries(SpinningObject ${OpenGlLinkers})


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")


add_executable(SpinningObject main.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

target_link_libraries(SpinningObject ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})