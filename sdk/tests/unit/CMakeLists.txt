set(TARGET_NAME "sdk_utests")

add_executable(${TARGET_NAME}
    AsyncResult_tests.cpp
    AsyncSubscription_tests.cpp
    DataPoint_tests.cpp
    DataPointBatch_tests.cpp
    Job_tests.cpp
    Logger_tests.cpp
    Node_tests.cpp
    ScopedBoolInverter_tests.cpp
    ThreadPool_tests.cpp
    Utils_tests.cpp
    VehicleDataBrokerClient_tests.cpp
    QueryBuilder_tests.cpp
    #PubSub_tests.cpp
)

add_dependencies(${TARGET_NAME} 
    vehicle-app-sdk
)

target_link_libraries(${TARGET_NAME} 
    vehicle-app-sdk
    gtest_main
    gmock
)

include(GoogleTest)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../mocks)

if(NOT CMAKE_TOOLCHAIN_FILE)
    gtest_discover_tests(${TARGET_NAME})
endif()
