cmake_minimum_required(VERSION 3.24)

project(gluck CXX)



message("building project")

file(GLOB_RECURSE SRC_FILES main.cpp)


include_directories(src)
add_executable(${PROJECT_NAME} ${SRC_FILES})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# add_executable(${PROJECT_NAME} src/main.cpp ${GLAD_SOURCE})

message("Adding glad:")
find_package(glad CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)



find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)


message("Adding glm:")

find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

message("Addding gli") 
find_package(gli CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE gli)

message("Addding assimp")
find_package(assimp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)

find_path(TINYGLTF_INCLUDE_DIRS "tiny_gltf.h")
target_include_directories(${PROJECT_NAME} PRIVATE ${TINYGLTF_INCLUDE_DIRS})

add_custom_target(copy-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
)
