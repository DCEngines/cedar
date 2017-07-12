set(LIBRARY_LIST glog gflags)

include_directories(${PROJECT_SOURCE_DIR}/cedar)

add_executable(cedar cedar.cc)
target_link_libraries(cedar ${LIBRARY_LIST})

add_executable(mkcedar mkcedar.cc)
target_link_libraries(mkcedar ${LIBRARY_LIST})

add_executable(create_find create_find.cc)
target_link_libraries(create_find ${LIBRARY_LIST})

