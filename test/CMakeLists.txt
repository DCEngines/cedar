set(LIBRARY_LIST gtest glog)

include_directories(${PROJECT_SOURCE_DIR}/cedar)

add_executable(cedar_test 
	cedar_test.cc)

target_link_libraries(cedar_test 
	${LIBRARY_LIST})

add_test(cedar_tests cedar_test)
