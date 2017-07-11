set(LIBRARY_LIST glog gflags)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(cedar cedar.cc)
target_link_libraries(cedar ${LIBRARY_LIST})

add_executable(mkcedar mkcedar.cc)
target_link_libraries(mkcedar ${LIBRARY_LIST})

#add_executable(bench bench.cc)
#target_link_libraries(bench ${LIBRARY_LIST})

#add_executable(bench_static bench_static.cc)
#target_link_libraries(bench_static ${LIBRARY_LIST})
