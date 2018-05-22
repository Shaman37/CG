cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)
set( CMAKE_CXX_FLAGS "-Wall -O3 -g -lGLEW" )



project(engine)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(DevIL REQUIRED)
link_libraries(${IL_LIBRARIES} )
include_directories(§{IL_INCLUDE_DIR})

set(ENGINE_FILES ./engine/engine.cpp ./engine/tinyxml2.cpp ./engine/group.cpp ./engine/transforms.cpp ./engine/parser.cpp  ./src/vertex.cpp ./src/shape.cpp ./engine/lights.cpp ./engine/global.h)

add_executable(engine ${ENGINE_FILES})
target_link_libraries(engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

project(generator)
set(GENERATOR_FILES ./generator/generator.cpp ./generator/models.cpp ./src/vertex.cpp ./src/shape.cpp ./generator/bezier.cpp)

add_executable(generator ${GENERATOR_FILES})
target_link_libraries(generator ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
