add_executable(lfq_tests 
    LFQTests.cpp
)

target_link_libraries(lfq_tests PRIVATE ringbuffer GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(lfq_tests)