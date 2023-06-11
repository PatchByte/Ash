project("AshTest")

file(GLOB ASH_TEST_SOURCE_FILES
    "source/*.*"
    "source/*/*.*"
    "source/*/*/*.*"
)

include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/source/)

add_executable(${PROJECT_NAME} ${ASH_TEST_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} Ash::Ash AshObjects::AshObjects)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

if(ASH_ENABLE_LOGGER)
    target_link_libraries(${PROJECT_NAME} AshLogger::AshLogger fmt::fmt)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ASH_ENABLE_LOGGER=1)
endif(ASH_ENABLE_LOGGER)

add_test(
    NAME ${PROJECT_NAME}
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
)