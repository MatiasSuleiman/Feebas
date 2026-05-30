cmake_minimum_required(VERSION 3.16)

project(feebas_particle_simulator LANGUAGES CXX)

option(FEEBAS_BUILD_SERVER "Build the local HTTP server" ON)
option(FEEBAS_BUILD_TESTS "Build C++ tests" ON)

add_custom_target(clean-compiled
  COMMAND ${CMAKE_COMMAND}
    "-DFEEBAS_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}"
    "-DFEEBAS_PRESERVE_BINARY_DIR=${CMAKE_BINARY_DIR}"
    -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/clean-local.cmake"
  COMMENT "Removing local CMake build outputs outside this active build tree"
  VERBATIM
)

add_library(feebas-engine
  src/particles.cpp
  src/world.cpp
  src/world_to_json_visitor.cpp
)

target_compile_features(feebas-engine PUBLIC cxx_std_17)

target_include_directories(feebas-engine PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(FEEBAS_BUILD_SERVER)
  add_executable(feebas-server
    src/main.cpp
  )

  target_compile_features(feebas-server PRIVATE cxx_std_17)

  find_path(CPP_HTTPLIB_INCLUDE_DIR httplib.h REQUIRED)
  find_library(CPP_HTTPLIB_LIBRARY NAMES cpp-httplib httplib REQUIRED)

  target_include_directories(feebas-server PRIVATE
    ${CPP_HTTPLIB_INCLUDE_DIR}
  )

  target_link_libraries(feebas-server PRIVATE
    feebas-engine
    ${CPP_HTTPLIB_LIBRARY}
  )
endif()

if(FEEBAS_BUILD_TESTS)
  find_package(Catch2 REQUIRED)

  file(GLOB_RECURSE FEEBAS_TEST_SOURCES CONFIGURE_DEPENDS
    tests/*.cpp
  )

  enable_testing()

  foreach(FEEBAS_TEST_SOURCE IN LISTS FEEBAS_TEST_SOURCES)
    file(RELATIVE_PATH FEEBAS_TEST_PATH
      ${CMAKE_CURRENT_SOURCE_DIR}
      ${FEEBAS_TEST_SOURCE}
    )

    string(REPLACE "/" "_" FEEBAS_TEST_NAME ${FEEBAS_TEST_PATH})
    string(REPLACE "." "_" FEEBAS_TEST_NAME ${FEEBAS_TEST_NAME})
    set(FEEBAS_TEST_TARGET feebas-test-${FEEBAS_TEST_NAME})

    add_executable(${FEEBAS_TEST_TARGET}
      ${FEEBAS_TEST_SOURCE}
    )

    target_link_libraries(${FEEBAS_TEST_TARGET} PRIVATE
      feebas-engine
    )

    if(TARGET Catch2::Catch2WithMain)
      target_link_libraries(${FEEBAS_TEST_TARGET} PRIVATE
        Catch2::Catch2WithMain
      )
    else()
      target_compile_definitions(${FEEBAS_TEST_TARGET} PRIVATE
        CATCH_CONFIG_MAIN
      )
      target_link_libraries(${FEEBAS_TEST_TARGET} PRIVATE
        Catch2::Catch2
      )
    endif()

    add_test(NAME ${FEEBAS_TEST_TARGET} COMMAND ${FEEBAS_TEST_TARGET})
  endforeach()
endif()
