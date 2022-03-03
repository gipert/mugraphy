# List here manually all source files. Using GLOB is bad, see:
# https://cmake.org/cmake/help/latest/command/file.html?highlight=Note#filesystem

set(_root ${CMAKE_SOURCE_DIR})

set(PROJECT_PUBLIC_HEADERS
    ${_root}/include/MUGNavigationTools.hh
    ${_root}/include/MUGGenerator.hh
    ${_root}/include/MUGLog.hh
    ${_root}/include/MUGLog.icc
    ${_root}/include/ProjectInfo.hh
    ${_root}/include/MUGDetectorConstruction.hh
    ${_root}/include/MUGPanelHit.hh
    ${_root}/include/MUGPanelSD.hh
    ${_root}/include/MUGRunAction.hh
    ${_root}/include/MUGEventAction.hh
    ${_root}/include/MUGStackingAction.hh
    ${_root}/include/MUGSteppingAction.hh
    ${_root}/include/MUGTrackingAction.hh
    ${_root}/include/MUGUserAction.hh
    ${_root}/include/MUGManager.hh
    ${_root}/include/MUGRun.hh
    ${_root}/include/MUGMaterialTable.hh
    ${_root}/include/MUGUIcmdStepLimit.hh
    ${_root}/include/MUGPhysics.hh
    ${_root}/include/MUGTools.hh
    ${_root}/include/MUGTools.icc
)

set(PROJECT_SOURCES
    ${_root}/src/MUGNavigationTools.cc
    ${_root}/src/MUGGenerator.cc
    ${_root}/src/MUGLog.cc
    ${_root}/src/MUGDetectorConstruction.cc
    ${_root}/src/MUGPanelHit.cc
    ${_root}/src/MUGPanelSD.cc
    ${_root}/src/MUGEventAction.cc
    ${_root}/src/MUGRunAction.cc
    ${_root}/src/MUGStackingAction.cc
    ${_root}/src/MUGSteppingAction.cc
    ${_root}/src/MUGTrackingAction.cc
    ${_root}/src/MUGUserAction.cc
    ${_root}/src/MUGManager.cc
    ${_root}/src/MUGMaterialTable.cc
    ${_root}/src/MUGPhysics.cc
    ${_root}/src/MUGUIcmdStepLimit.cc
)

add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_PUBLIC_HEADERS}
    ${PROJECT_SOURCES} ${CMAKE_PROJECT_NAME}.cc)

# link against dependent libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    PUBLIC ${Geant4_LIBRARIES})

if(ROOT_FOUND)
    target_link_libraries(${CMAKE_PROJECT_NAME}
        PUBLIC ROOT::Core ROOT::Tree ROOT::Hist)
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

# install GDML files
install(DIRECTORY geom/
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${CMAKE_PROJECT_NAME}
    FILES_MATCHING PATTERN "*.gdml"
    PATTERN "[_.]*" EXCLUDE) # exclude directories starting with '.' or '_'
