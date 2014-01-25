FILE (GLOB_RECURSE project_SRCS *.cpp *.cxx *.cc *.C *.c *.h *.hpp)
SET (project_LIBS ${X11_LIBRARIES} ${ASSIMP_LIBRARIES} ${PNG_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
SET (project_BIN ${PROJECT_NAME})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
ADD_EXECUTABLE(${project_BIN} ${project_SRCS})
TARGET_LINK_LIBRARIES(${project_BIN} ${project_LIBS})
