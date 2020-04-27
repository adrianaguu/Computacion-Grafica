PROJECT(Programa1)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(OpenGLProyect_SRC "src/glad.c")

link_directories(${CMAKE_SOURCE_DIR}/primer_programa//programa1/libraries)

INCLUDE_DIRECTORIES("primer_programa/programa1/include")

MESSAGE("Library Added Succesfully")

ADD_EXECUTABLE(OpenGLProyect ${OpenGLProyect_SRC})

TARGET_LINK_LIBRARIES(Programa1 PUBLIC glfw3)
TARGET_LINK_LIBRARIES(Programa1 PUBLIC opengl32)
find_package(OpenGL REQUIRED)