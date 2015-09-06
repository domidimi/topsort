include(gtest_ext)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${GTEST_INCLUDE_DIRS}
)

set(test_srcs
    topsort_utest.cpp
)

add_executable("${PROJECT_NAME}_tests" ${test_srcs})
target_link_libraries("${PROJECT_NAME}_tests"
    ${GTEST_LIBRARIES}
)
add_test(${PROJECT_NAME}_tests ${PROJECT_NAME}_tests)
