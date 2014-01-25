FILE (GLOB_RECURSE project_SRCS *.cpp *.cxx *.cc *.C *.c *.h *.hpp)
#SET (project_LIBS ${Boost_LIBRARIES} ${Log4Cxx_LIBRARIES})
SET (project_BIN ${PROJECT_NAME})

ADD_EXECUTABLE(${PROJECT_NAME} ${project_SRCS})