set(TEST_EXE_NAME test_generator)

add_executable(${TEST_EXE_NAME} ${TEST_EXE_NAME}.cc)

target_link_libraries(${TEST_EXE_NAME} PUBLIC ${Geant4_LIBRARIES} ROOT::Core ROOT::RIO ROOT::Tree)

target_include_directories(${TEST_EXE_NAME}
                           PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>")

install(TARGETS ${TEST_EXE_NAME})
