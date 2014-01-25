FILE (GLOB_RECURSE project_SRCS *.cpp *.cxx *.cc *.C *.c *.h *.hpp)
SET (project_LIBS ${X11_LIBRARIES})
SET (project_BIN ${PROJECT_NAME})

ADD_EXECUTABLE(${PROJECT_NAME} ${project_SRCS} ${project_LIBS})