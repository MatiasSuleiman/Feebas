cmake_minimum_required(VERSION 3.16)

project(feebas_particle_simulator LANGUAGES CXX)

option(FEEBAS_BUILD_SERVER "Build the local HTTP server" ON)
option(FEEBAS_BUILD_TESTS "Build C++ tests" ON)

add_library(feebas-engine
  src/particles.cpp
  src/world.cpp
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

  add_executable(feebas-tests
    tests/particle_tests/test01.cpp
  )

  target_link_libraries(feebas-tests PRIVATE
    feebas-engine
  )

  if(TARGET Catch2::Catch2WithMain)
    target_link_libraries(feebas-tests PRIVATE
      Catch2::Catch2WithMain
    )
  else()
    target_compile_definitions(feebas-tests PRIVATE
      CATCH_CONFIG_MAIN
    )
    target_link_libraries(feebas-tests PRIVATE
      Catch2::Catch2
    )
  endif()

  enable_testing()
  add_test(NAME feebas-tests COMMAND feebas-tests)
endif()
