project("Ash")

file(GLOB ASH_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/Ash/*.h)
file(GLOB ASH_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/include/)

add_library(${PROJECT_NAME} STATIC  ${ASH_HEADER_FILES}
                                    ${ASH_SOURCE_FILES}
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

if(ASH_OBJECTS_ERASE_NAMES)
    target_compile_definitions(${PROJECT_NAME} PUBLIC ASH_OBJECTS_ERASE_NAMES=1)
endif(ASH_OBJECTS_ERASE_NAMES)