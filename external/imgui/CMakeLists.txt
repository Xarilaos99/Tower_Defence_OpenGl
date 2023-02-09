cmake_minimum_required (VERSION 2.6)
project (IMGUI)




find_package (OpenGL REQUIRED)
include_directories (${OPENGL_INCLUDE_DIR})
set(LIBRARIES ${OPENGL_LIBRARIES})

list(APPEND IMGUI_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

list(APPEND IMGUI_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)


include_directories(include/)

add_library (IMGUI ${IMGUI_SOURCES} ${IMGUI_HEADERS})
target_link_libraries (IMGUI ${LIBRARIES})
set_target_properties(IMGUI
    PROPERTIES
    PROJECT_LABEL imgui
    FOLDER "Libraries"
)

install (TARGETS IMGUI
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)
install (FILES ${IMGUI_HEADERS} DESTINATION include/IMGUI)

add_library(${PROJECT_NAME} STATIC ${IMGUI_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../glfw/include)