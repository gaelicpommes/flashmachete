cmake_minimum_required(VERSION 3.16...3.27)
project(FlashElectronSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Geant4 with UI + Vis
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

# Collect sources/headers
file(GLOB sources CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(FlashElectronSim ${sources} ${headers})

# Include path
target_include_directories(FlashElectronSim PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Link Geant4
target_link_libraries(FlashElectronSim ${Geant4_LIBRARIES})

# ---- Copy macros into build/macros ----
file(GLOB MACRO_FILES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/macros/*.mac")

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/macros)

foreach(_file ${MACRO_FILES})
  get_filename_component(_fname ${_file} NAME)
  configure_file(${_file} ${PROJECT_BINARY_DIR}/macros/${_fname} COPYONLY)
endforeach()


