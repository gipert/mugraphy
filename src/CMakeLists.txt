# List here manually all source files. Using GLOB is bad, see:
# https://cmake.org/cmake/help/latest/command/file.html?highlight=Note#filesystem

set(PROJECT_PUBLIC_HEADERS
    ../include/MUGNavigationTools.hh
    ../include/MUGGenerator.hh
    ../include/MUGLog.hh
    ../include/MUGLog.icc
    ../include/ProjectInfo.hh
    ../include/MUGDetectorConstruction.hh
    ../include/MUGRunAction.hh
    ../include/MUGEventAction.hh
    ../include/MUGStackingAction.hh
    ../include/MUGSteppingAction.hh
    ../include/MUGTrackingAction.hh
    ../include/MUGUserAction.hh
    ../include/MUGManager.hh
    ../include/MUGRun.hh
    ../include/MUGMaterialTable.hh
    ../include/MUGUIcmdStepLimit.hh
    ../include/MUGProcessesList.hh
    ../include/MUGProcessesMessenger.hh
    ../include/MUGTools.hh
    ../include/MUGTools.icc
)

set(PROJECT_SOURCES
    MUGNavigationTools.cc
    MUGGenerator.cc
    MUGLog.cc
    MUGDetectorConstruction.cc
    MUGEventAction.cc
    MUGRunAction.cc
    MUGStackingAction.cc
    MUGSteppingAction.cc
    MUGTrackingAction.cc
    MUGUserAction.cc
    MUGManager.cc
    MUGMaterialTable.cc
    MUGProcessesList.cc
    MUGProcessesMessenger.cc
    MUGUIcmdStepLimit.cc
    mugraphy.cc
)

add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_PUBLIC_HEADERS} ${PROJECT_SOURCES})

# link against dependent libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    PUBLIC
        ${Geant4_LIBRARIES})

if(ROOT_FOUND)
    target_link_libraries(${CMAKE_PROJECT_NAME}
        PUBLIC
            ROOT::Core)
endif()

# Ensure clients are aware of the minimum C++ standard we were compiled with
target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_${CMAKE_CXX_STANDARD})

target_include_directories(${CMAKE_PROJECT_NAME}
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>")

# write ProjectInfo.hh
configure_file(${CMAKE_SOURCE_DIR}/cmake/ProjectInfo.hh.in
    ${CMAKE_SOURCE_DIR}/include/ProjectInfo.hh @ONLY)

install(TARGETS ${CMAKE_PROJECT_NAME})
