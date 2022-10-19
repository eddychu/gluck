cmake_minimum_required(VERSION 3.24)

project(gluck)

message("building project")

file(GLOB_RECURSE SRC_FILES "src/*.h" "src/*.cpp")
set(GLAD_SOURCE extern/glad/src/glad.c)

include_directories(src)
add_executable(${PROJECT_NAME} ${SRC_FILES} ${GLAD_SOURCE})

# add_executable(${PROJECT_NAME} src/main.cpp ${GLAD_SOURCE})

message("Adding glad:")
target_include_directories(${PROJECT_NAME} PRIVATE extern/glad/include)


set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_INSTALL OFF CACHE BOOL "")
set(GLFW_DIR extern/glfw)

add_subdirectory(${GLFW_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

# set_property(TARGET glfw PROPERTY FOLDER "extern/glfw")
# set_property(TARGET update_mappings PROPERTY FOLDER "extern/glfw")



message("Adding glm:")

set(GLM_DIR extern/glm)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLM_DIR})

message("Addding gli") 
set(GLI_DIR extern/gli)
add_subdirectory(${GLI_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${GLI_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE gli)

message("Addding assimp")
find_package(assimp REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${ASSIMP_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${ASSIMP_LIBRARIES})



add_custom_target(copy-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
)
