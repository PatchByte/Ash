project("AshTest")

file(GLOB ASH_TEST_SOURCE_FILES
    "source/*.*"
    "source/*/*.*"
)

add_executable(${PROJECT_NAME} ${ASH_TEST_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} Ash::Ash AshObjects::AshObjects)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
