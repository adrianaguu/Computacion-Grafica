PROJECT(Programa1)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(Programa1_SRC "${CMAKE_SOURCE_DIR}/primer_programa/programa1/programa1/Source.cpp")

link_directories(${CMAKE_SOURCE_DIR}/primer_programa//programa1/libraries)

INCLUDE_DIRECTORIES("primer_programa/programa1/include")

MESSAGE("Library Added Succesfully")

ADD_EXECUTABLE(Programa1 ${Programa1_SRC})

TARGET_LINK_LIBRARIES(Programa1 PUBLIC glfw3)
TARGET_LINK_LIBRARIES(Programa1 PUBLIC opengl32)
find_package(OpenGL REQUIRED)