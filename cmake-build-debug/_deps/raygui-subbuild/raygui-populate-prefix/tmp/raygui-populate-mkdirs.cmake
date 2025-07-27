# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "E:/DayTradingSimulator/cmake-build-debug/_deps/raygui-src")
  file(MAKE_DIRECTORY "E:/DayTradingSimulator/cmake-build-debug/_deps/raygui-src")
endif()
file(MAKE_DIRECTORY
  "E:/DayTradingSimulator/cmake-build-debug/_deps/raygui-build"
  "E:/DayTradingSimulator/cmake-build-debug/_deps/raygui-subbuild/raygui-populate-prefix"
  "E:/DayTradingSimulator/cmake-build-debug/_deps/raygui-subbuild/raygui-populate-prefix/tmp"
  "E:/DayTradingSimulator/cmake-build-debug/_deps/raygui-subbuild/raygui-populate-prefix/src/raygui-populate-stamp"
  "E:/DayTradingSimulator/cmake-build-debug/_deps/raygui-subbuild/raygui-populate-prefix/src"
  "E:/DayTradingSimulator/cmake-build-debug/_deps/raygui-subbuild/raygui-populate-prefix/src/raygui-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/DayTradingSimulator/cmake-build-debug/_deps/raygui-subbuild/raygui-populate-prefix/src/raygui-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/DayTradingSimulator/cmake-build-debug/_deps/raygui-subbuild/raygui-populate-prefix/src/raygui-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
