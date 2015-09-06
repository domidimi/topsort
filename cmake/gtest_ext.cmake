option(USE_SYSTEM_GTEST "Use system gtest when set to ON" OFF)

if(USE_SYSTEM_GTEST)
  find_package(GTest)
  return()
endif()

find_package(Threads)
include(ExternalProject)

set(GTEST_LIBRARIES gtest gtest_main)

set(byproducts)
foreach(lib ${GTEST_LIBRARIES})
  set(${lib}_path
    <BINARY_DIR>/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${lib}${CMAKE_STATIC_LIBRARY_SUFFIX})
  list(APPEND byproducts ${${lib}_path})
endforeach()

# Set default ExternalProject root directory
ExternalProject_Add(gtest_ext
    URL http://googletest.googlecode.com/files/gtest-1.7.0.zip
    URL_MD5 2d6ec8ccdf5c46b05ba54a9fd1d130d7
    # If using the Git repo a check will be re-triggered on each execution
    # GIT_REPOSITORY "https://github.com/google/googletest.git"
    # GIT_TAG "release-1.7.0"
    PREFIX "${CMAKE_BINARY_DIR}/ThirdParty"
    INSTALL_COMMAND ""
    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    # Needed for the Ninja generator to work
    BUILD_BYPRODUCTS ${byproducts}
    # Forward any flags already set to make sure we are using the same flags as
    # for the rest of the project. That is the recommended way for using
    # googletest
    CMAKE_CACHE_ARGS
      -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
      -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
      -DCMAKE_CXX_FLAGS_DEBUG:STRING=${CMAKE_CXX_FLAGS_DEBUG}
      -DCMAKE_CXX_FLAGS_MINSIZEREL:STRING=${CMAKE_CXX_FLAGS_MINSIZEREL}
      -DCMAKE_CXX_FLAGS_RELEASE:STRING=${CMAKE_CXX_FLAGS_RELEASE}
      -DCMAKE_CXX_FLAGS_RELWITHDEBINFO:STRING=${CMAKE_CXX_FLAGS_RELWITHDEBINFO}
      -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
      -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
      -DCMAKE_C_FLAGS_DEBUG:STRING=${CMAKE_C_FLAGS_DEBUG}
      -DCMAKE_C_FLAGS_MINSIZEREL:STRING=${CMAKE_C_FLAGS_MINSIZEREL}
      -DCMAKE_C_FLAGS_RELEASE:STRING=${CMAKE_C_FLAGS_RELEASE}
      -DCMAKE_C_FLAGS_RELWITHDEBINFO:STRING=${CMAKE_C_FLAGS_RELWITHDEBINFO}
      -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
)
ExternalProject_Get_Property(gtest_ext source_dir binary_dir)
foreach(lib ${GTEST_LIBRARIES})
    add_library(${lib} IMPORTED STATIC GLOBAL)
    add_dependencies(${lib} gtest_ext)
    _ep_replace_location_tags(gtest_ext ${lib}_path)
    set_target_properties(${lib} PROPERTIES
        "IMPORTED_LOCATION" ${${lib}_path}
        "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
        )
endforeach()

set(GTEST_INCLUDE_DIRS ${source_dir}/include)
set(GTEST_FOUND ON)
