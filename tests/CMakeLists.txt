add_executable(skiplist_test skiplist_test.cpp)
target_link_libraries(skiplist_test PRIVATE dkvs::core)
add_test(NAME skiplist_test COMMAND skiplist_test)
